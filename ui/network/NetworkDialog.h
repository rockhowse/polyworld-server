#ifndef NETWORKDIALOG_H
#define NETWORKDIALOG_H

#include <QWidget>
#include "NetworkServer.h"

class QLabel;
class QPushButton;

class NetworkDialog : public QWidget
{
    Q_OBJECT

public:
    NetworkDialog(QWidget *parent = 0);

private:
    QLabel *statusLabel;
    QPushButton *quitButton;
    NetworkServer server;
};

#endif // NETWORKDIALOG_H
