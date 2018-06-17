#include "incrementaldatatransfer.h"
#include <QIODevice>
#include <QTimer>
#include <QDebug>

// Block-size is 64KB. I wondered if that should be configurable per-object,
// but decided not to (needless complexity, plus it would require a dynamic allocation).
#define BLOCK_SIZE 64*1024

IncrementalDataTransfer::IncrementalDataTransfer(QIODevice *src, QIODevice *dest, QObject *parent) : QObject(parent)
{
    this->src = src;
    this->dest = dest;

    // When new data is available for reading, or if a block of data was successfully written,
    // transfer another block. Note that both of these signals work for e.g. sockets but not for
    // files.
    connect(src, &QIODevice::readyRead, this, &IncrementalDataTransfer::transferAnotherBlock);
    connect(dest, &QIODevice::bytesWritten, this, &IncrementalDataTransfer::transferAnotherBlock);

    // Take ownership of both ends. I think this is correct, since we assume both ends should exist
    // only with the purpose of data transfer. I doubt they will be used for any other purpose.
    // Setting them as parents also means they will be destroyed when the data transfer ends.
    src->setParent(this);
    dest->setParent(this);

    // If one of the devices is destroyed, we end the data transfer.
    connect(src, &QObject::destroyed, this, &QObject::deleteLater);
    connect(dest, &QObject::destroyed, this, &QObject::deleteLater);

    // QFile doesn't emit the bytesWritten signal, so we need to use a QTimer as a workaround.
    timer = 0;
    qDebug() << dest->metaObject()->className();
    if (dest->metaObject()->className() == QString("QFile")) {
        timer = new QTimer(this);
        timer->setInterval(0);
        timer->setSingleShot(true);
        connect(timer, &QTimer::timeout, this, &IncrementalDataTransfer::transferAnotherBlock);
    }

    // Kick-start the process.
    transferAnotherBlock();
}

void IncrementalDataTransfer::transferAnotherBlock()
{
    if (src->bytesAvailable() && !dest->bytesToWrite()) {
        // Since BLOCK_SIZE is hardcoded we can use a static array
        // instead of a QByteArray.
        char buffer[BLOCK_SIZE];
        int n = src->read(buffer, BLOCK_SIZE);
        dest->write(buffer, n);
    }

    if (src->atEnd()) {
        // Have we finished? End the data transfer.
        deleteLater();
    } else if (timer) {
        // // QFile doesn't emit the bytesWritten signal.
        timer->start();
    }
}
