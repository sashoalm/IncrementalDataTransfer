#include "mockiodevice.h"
#include <QTimer>
#include <QDebug>

MockIODevice::MockIODevice(int timeout, int blocksRemaining, int blockSize, bool reportAtEndInsteadOfClosing, QObject *parent) : QIODevice(parent)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MockIODevice::onTimeout);
    timer->setInterval(timeout);
    timer->setSingleShot(true);
    this->blocksRemaining = blocksRemaining;
    this->blockSize = blockSize;
    this->reportAtEndInsteadOfClosing = reportAtEndInsteadOfClosing;
    open(ReadWrite);
}

void MockIODevice::onTimeout()
{
    qDebug() << "a" << timer->isActive();
    emit readyRead();
    emit bytesWritten(blockSize);
}

qint64 MockIODevice::readData(char *data, qint64 maxlen)
{
    Q_UNUSED(data);
    qDebug("Mock read: %d bytes", (int) maxlen);
    if (!--blocksRemaining) {
        qDebug("Mock read: closing");
        if (!reportAtEndInsteadOfClosing) {
            deleteLater();
        }
    }
    return qMin(blockSize, int(maxlen));
}

qint64 MockIODevice::writeData(const char *data, qint64 len)
{
    Q_UNUSED(data);
    qDebug("Mock write: %d bytes", (int) len);
    if (!--blocksRemaining) {
        qDebug("Mock write: closing");
        deleteLater();
    }
    timer->start();
    return 1;
}

qint64 MockIODevice::bytesAvailable() const
{
    // Report no bytes available if we are waiting for the timer.
    return timer->isActive() ? 0 : blockSize;
}

qint64 MockIODevice::bytesToWrite() const
{
    // Report we still have bytes to write if we are waiting for the timer.
    return timer->isActive() ? blockSize : 0;
}

bool MockIODevice::atEnd() const
{
    return !blocksRemaining;
}

