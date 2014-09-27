#ifndef NETWORK_THREAD_H
#define NETWORK_THREAD_H

#include <QThread>
#include <QTcpSocket>

class NetworkThread : public QThread
{
    Q_OBJECT

public:
    NetworkThread(int socketDescriptor, const QString &polyworldMessage, QObject *parent);

    void run();

signals:
    void error(QTcpSocket::SocketError socketError);

private:
    int socketDescriptor;
    QString text;
    bool clientConnected;
};

#endif // NETWORK_THREAD_H
