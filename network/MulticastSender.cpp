/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtWidgets>
#include <QtNetwork>

#include "MulticastSender.h"

MulticastSender::MulticastSender(QWidget *parent)
    : QDialog(parent)
{
    groupAddress = QHostAddress("239.255.43.21");

    statusLabel = new QLabel(tr("Ready to multicast datagrams to group %1 on port 45454").arg(groupAddress.toString()));

    ttlLabel = new QLabel(tr("TTL for multicast datagrams:"));
    ttlSpinBox = new QSpinBox;
    ttlSpinBox->setRange(0, 255);

    QHBoxLayout *ttlLayout = new QHBoxLayout;
    ttlLayout->addWidget(ttlLabel);
    ttlLayout->addWidget(ttlSpinBox);

    startButton = new QPushButton(tr("&Start"));
    quitButton = new QPushButton(tr("&Quit"));

    buttonBox = new QDialogButtonBox;
    buttonBox->addButton(startButton, QDialogButtonBox::ActionRole);
    buttonBox->addButton(quitButton, QDialogButtonBox::RejectRole);

    //timer = new QTimer(this);
    udpSocket = new QUdpSocket(this);
    messageNo = 1;

    connect(ttlSpinBox, SIGNAL(valueChanged(int)), this, SLOT(ttlChanged(int)));
    connect(startButton, SIGNAL(clicked()), this, SLOT(startSending()));
    connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
    //connect(timer, SIGNAL(timeout()), this, SLOT(sendDatagram()));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(statusLabel);
    mainLayout->addLayout(ttlLayout);
    mainLayout->addWidget(buttonBox);
    setLayout(mainLayout);

    setWindowTitle(tr("Multicast Sender"));
    ttlSpinBox->setValue(1);
    sendMulticast = true;
}

void MulticastSender::ttlChanged(int newTtl)
{
    udpSocket->setSocketOption(QAbstractSocket::MulticastTtlOption, newTtl);
}

/**
 * Server side step has completed. Send the agent info to the clients
 *
 * @brief MulticastSender::setStep
 * @param curStep
 * @param sendAgent
 */
void MulticastSender::setStep(int curStep, agent * sendAgent)
{
    simStep = curStep;

    if(sendMulticast) {
        // send the dataGram
        sendDatagram(sendAgent,MSG_TYPE_STEP);
    }
}

/**
 * Notify the clients of an agent's birth
 *
 * @brief MulticastSender::agentBirthMsg
 * @param sendAgent
 */
void MulticastSender::agentBirthMsg(agent * sendAgent)
{
    if(sendMulticast) {
        // send the dataGram
        sendDatagram(sendAgent,MSG_TYPE_AGENT_BIRTH);
    }
}

/**
 * Notify the clients of an agent's death
 *
 * @brief MulticastSender::agentDeathMsg
 * @param sendAgent
 */
void MulticastSender::agentDeathMsg(agent * sendAgent)
{
    if(sendMulticast) {
        // send the dataGram
        sendDatagram(sendAgent,MSG_TYPE_AGENT_DEATH);
    }
}


void MulticastSender::startSending()
{
    sendMulticast = true;
    startButton->setEnabled(false);
    //timer->start(1000);
}

void MulticastSender::sendDatagram(agent * sendAgent, int msgType)
{

    switch(msgType) {

        // on a step send all agent locations
        case MSG_TYPE_STEP:

            // have to use explicit block due to
            // http://stackoverflow.com/questions/5685471/error-jump-to-case-label
            {
                struct SimStepHeader {
                    int simStep;
                    int agentCount;
                };

                struct SimAgentData {
                    long  agentNum;
                    float agentX;
                    float agentY;
                    float agentZ;
                    float agentYaw;
                };


                int agentCount = objectxsortedlist::gXSortedObjects.getCount(AGENTTYPE);

                SimStepHeader *ssh = new SimStepHeader();
                ssh->simStep = simStep;
                ssh->agentCount = agentCount;

                QByteArray datagram;
                QDataStream out(&datagram, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_3);
                out << MSG_TYPE_STEP
                    << ssh->simStep
                    << ssh->agentCount;

                SimAgentData *sad = new SimAgentData();

                agent *a;

                objectxsortedlist::gXSortedObjects.reset();
                while (objectxsortedlist::gXSortedObjects.nextObj(AGENTTYPE, (gobject**)&a))
                {
                    sad->agentNum = sendAgent->Number();
                    sad->agentX = sendAgent->x();
                    sad->agentY = sendAgent->y();
                    sad->agentZ = sendAgent->z();
                    sad->agentYaw = sendAgent->yaw();

                    out << qint64(sad->agentNum)
                        << sad->agentX
                        << sad->agentY
                        << sad->agentZ
                        << sad->agentYaw;
                }

                udpSocket->writeDatagram(datagram, groupAddress, 45454);

                delete(sad);
                delete(ssh);
            }
        break;

        // on a birth, send the following data from the agent so it can be created on the other side.
        //
        // 1. agentID
        // 2. height
        // 3. width
        // 4. age (?)
        // ?. anything else set at birth
        case MSG_TYPE_AGENT_BIRTH:
            {
                struct AgentBirthPacket {
                    long    agentNum;
                    float   agentHeight;
                    float   agentSize;
                };

                AgentBirthPacket * abp = new AgentBirthPacket();
                abp->agentNum       = sendAgent->Number();
                abp->agentHeight    = sendAgent->Height();
                abp->agentSize      = sendAgent->Size();

                QByteArray datagram;
                QDataStream out(&datagram, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_3);
                out << MSG_TYPE_AGENT_BIRTH
                    << qint64(abp->agentNum)
                    << abp->agentHeight
                    << abp->agentSize;

                udpSocket->writeDatagram(datagram, groupAddress, 45454);

                delete(abp);
            }
            break;

        // On a death, send the following data so the agent can be removed from the client
        // 1. agentID
        // ?. anything else needed to smite the agent
        case MSG_TYPE_AGENT_DEATH:
            {
                struct AgentDeathPacket {
                    long    agentNum;
                };

                AgentDeathPacket * adp = new AgentDeathPacket();
                adp->agentNum       = sendAgent->Number();

                QByteArray datagram;
                QDataStream out(&datagram, QIODevice::WriteOnly);
                out.setVersion(QDataStream::Qt_4_3);
                out << MSG_TYPE_AGENT_DEATH
                    << qint64(adp->agentNum);

                udpSocket->writeDatagram(datagram, groupAddress, 45454);

                delete(adp);
            }
            break;
    }
}
