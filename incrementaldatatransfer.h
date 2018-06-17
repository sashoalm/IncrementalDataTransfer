#ifndef INCREMENTALDATATRANSFER_H
#define INCREMENTALDATATRANSFER_H

#include <QObject>

class QIODevice;
class QTimer;

/**
 * @brief The IncrementalDataTransfer class transfers large amounts of data, block-by-block.
 *
 * This class facilitates big data transfers from any generic QIODevice to another - it
 * could be socket to file, socket to socket, file to socket etc. The problem it solves
 * is if you want to transfer a 1GB file to a socket - doing that by reading all the
 * 1 GB to memory and dumping it to the socket would not be a good approach. Instead,
 * this class will read it incrementally, block by block (block size is 64KB), so we
 * don't overload the RAM. We use events, so the IncrementalDataTransfer does not need a separate
 * thread, and multiple IncrementalDataTransfer classes can coexist in the same thread.
 */
class IncrementalDataTransfer : public QObject
{
    Q_OBJECT
public:
    explicit IncrementalDataTransfer(QIODevice *src, QIODevice *dest, QObject *parent = 0);

signals:

private slots:
    void transferAnotherBlock();

private:
    QIODevice *src;
    QIODevice *dest;
    QTimer *timer;
};

#endif // INCREMENTALDATATRANSFER_H
