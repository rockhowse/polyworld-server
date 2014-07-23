#ifndef SENDWORLDFILETHREAD_H
#define SENDWORLDFILETHREAD_H

#include <QThread>
#include <QFile>
#include <QTcpSocket>

class SendWorldFileThread : public QThread
{
public:
    SendWorldFileThread(qintptr socketdescriptor,QObject *parent);
    void run();

private:
    int socketDescriptor;
};

#endif // SENDWORLDFILETHREAD_H
