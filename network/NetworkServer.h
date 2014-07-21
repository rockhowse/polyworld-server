#ifndef NETWORK_SERVER_H
#define NETWORK_SERVER_H

#include <QStringList>
#include <QTcpServer>

class NetworkServer : public QTcpServer
{
    Q_OBJECT

public:
    NetworkServer(QObject *parent = 0);

protected:
    void incomingConnection(int socketDescriptor);

private:
    QStringList polyworldMessages;
};

#endif //NETWORK_SERVER_H
