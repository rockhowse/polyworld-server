#include "SendWorldFileThread.h"
#define FILENAME "./sandbox/social/feed_young.wf"

SendWorldFileThread::SendWorldFileThread(qintptr socketDescriptor,QObject *parent)
    : QThread(parent), socketDescriptor(socketDescriptor)
{
}

void SendWorldFileThread::run()
{
    QTcpSocket client;
    qDebug() << "Thread Descriptor :" << socketDescriptor;
    if (!client.setSocketDescriptor(socketDescriptor))
    {
        qDebug() << client.error();
        return;
    }
    qDebug() << "Thread : Connected";

    //send File
    QFile inputFile(FILENAME);
    QByteArray read;
    inputFile.open(QIODevice::ReadOnly);
    while(true)
    {
        read.clear();
        read = inputFile.read(32768*8);
        qDebug() << "Read : " << read.size();
        if(read.size()==0)
            break;

        qDebug() << "Written : " << client.write(read);
        client.waitForBytesWritten();
        read.clear();
    }
    inputFile.close();
    client.disconnectFromHost();
    client.waitForDisconnected();
    qDebug() << "Thread : File transfer completed";
}
