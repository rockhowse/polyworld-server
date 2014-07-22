
#include "NetworkServer.h"
#include "NetworkThread.h"

#include <stdlib.h>


NetworkServer::NetworkServer(QObject *parent)
    : QTcpServer(parent)
{
    polyworldMessages << tr("Polyworld message 1")
                      << tr("Polyworld message 2")
                      << tr("Polyworld message 3")
                      << tr("Polyworld message 4")
                      << tr("Polyworld message 5")
                      << tr("Polyworld message 6")
                      << tr("Polyworld message 7");
}

void NetworkServer::incomingConnection(qintptr socketDescriptor)
{
    QString polyworldMessage = polyworldMessages.at(qrand() % polyworldMessages.size());
    NetworkThread *thread = new NetworkThread(socketDescriptor, polyworldMessage, this);
    connect(thread, SIGNAL(finished()), thread, SLOT(deleteLater()));
    thread->start();
}
