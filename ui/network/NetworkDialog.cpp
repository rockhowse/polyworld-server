
#include <QtGui>
#include <QtNetwork>

#include <stdlib.h>

#include "NetworkDialog.h"
#include "NetworkServer.h"
#include <QtWidgets>
#include <QtNetwork>

NetworkDialog::NetworkDialog(QWidget *parent)
    : QWidget(parent)
{
    statusLabel = new QLabel;
    statusLabel->setWordWrap(true);
    quitButton = new QPushButton(tr("Quit"));
    quitButton->setAutoDefault(false);

    QString ipAddressStr;
    QHostAddress ipAddress;

    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();
    // use the first non-localhost IPv4 address
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
            ipAddressesList.at(i).toIPv4Address()) {
            ipAddressStr = ipAddressesList.at(i).toString();
            ipAddress = ipAddressesList.at(i);
            break;
        }
    }
    // if we did not find one, use IPv4 localhost
    if (ipAddressStr.isEmpty())
        ipAddressStr = QHostAddress(QHostAddress::LocalHost).toString();

    if (!server.listen(ipAddress, 27000)) {
        QMessageBox::critical(this, tr("Threaded Polyworld Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server.errorString()));
        close();
        return;
    }


    statusLabel->setText(tr("The server is running on\n\nIP: %1\nport: %2\n\n"
                            "Run the Polyworld Client example now.")
                         .arg(ipAddressStr).arg(server.serverPort()));

    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch(1);
    buttonLayout->addWidget(quitButton);
    buttonLayout->addStretch(1);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);
    setWindowTitle(tr("Threaded Polyworld Server"));
}
