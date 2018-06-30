#ifndef MOCKIODEVICE_H
#define MOCKIODEVICE_H

#include <QIODevice>

class QTimer;

class MockIODevice : public QIODevice
{
    Q_OBJECT
public:
    explicit MockIODevice(int timeout, int blocksRemaining, int blockSize, bool reportAtEndInsteadOfClosing, QObject *parent = 0);

private slots:
    void onTimeout();

private:
    qint64 readData(char *data, qint64 maxlen);
    qint64 writeData(const char *data, qint64 len);
    QTimer *timer;
    int blocksRemaining;
    int blockSize;
    qint64 bytesAvailable() const;
    qint64 bytesToWrite() const;
    bool atEnd() const;
    bool reportAtEndInsteadOfClosing;
};

#endif // MOCKIODEVICE_H
