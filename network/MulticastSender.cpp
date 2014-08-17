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
void MulticastSender::simStepMsg(int curStep, agent * sendAgent, float sceneRotation)
{
    simStep = curStep;

    // Every step we want to send the following
    //
    // A. Header
    //  1. simulation step
    //  2. number of agents
    //  3. the current rotation of the Main scene
    //
    // B. AgentData
    //  1. agent's unique number
    //  2. agent's X position
    //  3. agent's Y position
    //  4. agent's Z position
    //  5. agent's Yaw
    //  6. agent's Red
    //  7. agent's Green
    //  8. agent's Blue
    //
    // C. FoodData
    // 1. food's unique number
    // 2. food's size

    if(sendMulticast) {

        // initialize the datagram
        QByteArray datagram;
        QDataStream out(&datagram, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);

        //////////// STEP HEADER /////////////
        int agentCount = objectxsortedlist::gXSortedObjects.getCount(AGENTTYPE);
        int foodCount = objectxsortedlist::gXSortedObjects.getCount(FOODTYPE);

        struct SimStepHeader {
            int simStep;
            int agentCount;
            int foodCount;
            float sceneRotation;
        };

        SimStepHeader *ssh = new SimStepHeader();
        ssh->simStep = simStep;
        ssh->agentCount = agentCount;
        ssh->foodCount = foodCount;
        ssh->sceneRotation = sceneRotation;

        out << MSG_TYPE_STEP
            << ssh->simStep
            << ssh->agentCount
            << ssh->sceneRotation;

        delete(ssh);

        ///////////////// Agent /////////////////
        struct SimAgentData {
            long  agentNum;
            float agentX;
            float agentY;
            float agentZ;
            float agentYaw;
            float agentRedChannel;
            float agentGreenChannel;
            float agentBlueChannel;
        };

        SimAgentData *sad = new SimAgentData();

        agent *a;

        objectxsortedlist::gXSortedObjects.reset();
        while (objectxsortedlist::gXSortedObjects.nextObj(AGENTTYPE, (gobject**)&a))
        {
            sad->agentNum = a->Number();
            sad->agentX = a->x();
            sad->agentY = a->y();
            sad->agentZ = a->z();
            sad->agentYaw = a->yaw();
            sad->agentRedChannel = a->GetRed();
            sad->agentGreenChannel = a->GetGreen();
            sad->agentBlueChannel = a->GetBlue();

            out << qint64(sad->agentNum)
                << sad->agentX
                << sad->agentY
                << sad->agentZ
                << sad->agentYaw
                << sad->agentRedChannel
                << sad->agentGreenChannel
                << sad->agentBlueChannel;
        }

        delete(sad);

        //////////////// FOOD ////////////////
        struct SimFoodData {
            long foodNum;
            float foodXLen;
            float foodYLen;
            float foodZLen;
        };

        SimFoodData *sfd = new SimFoodData();

        food *f;

        objectxsortedlist::gXSortedObjects.reset();
        while (objectxsortedlist::gXSortedObjects.nextObj(FOODTYPE, (gobject**)&f))
        {
            sfd->foodNum = f->Number();
            sfd->foodXLen = f->getlenx();
            sfd->foodYLen = f->getleny();
            sfd->foodZLen = f->getlenz();

            out << qint64(sfd->foodNum)
                << sfd->foodXLen
                << sfd->foodYLen
                << sfd->foodZLen;
        }

        delete(sfd);

        // send the data
        udpSocket->writeDatagram(datagram, groupAddress, 45454);
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
    // on a birth, send the following data from the agent so it can be created on the other side.
    //
    // 1. agentID
    // 2. height
    // 3. width
    // 4. age (?)
    // ?. anything else set at birth

    if(sendMulticast) {

        struct AgentBirthPacket {
            long    agentNum;
            float   agentHeight;
            float   agentSize;
            float   agentGeneCacheMaxSpeed;
        };

        AgentBirthPacket * abp = new AgentBirthPacket();
        abp->agentNum               = sendAgent->Number();
        abp->agentHeight            = sendAgent->Height();
        abp->agentSize              = sendAgent->Size();
        abp->agentGeneCacheMaxSpeed = sendAgent->GeneCacheMaxSpeed();

        QByteArray datagram;
        QDataStream out(&datagram, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << MSG_TYPE_AGENT_BIRTH
            << qint64(abp->agentNum)
            << abp->agentHeight
            << abp->agentSize
            << abp->agentGeneCacheMaxSpeed;

        udpSocket->writeDatagram(datagram, groupAddress, 45454);

        delete(abp);
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
    // On a death, send the following data so the agent can be removed from the client
    // 1. agentNum
    // ?. anything else needed to smite the agent

    if(sendMulticast) {

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
}

/**
 * This will send the data needed to add a food object to the scene
 *
 * @brief foodAddMessage
 * @param sendFood
 */
void MulticastSender::foodAddMsg(food * sendFood) {
    // on a birth, send the following data from the agent so it can be created on the other side.
    //
    // 1. foodNum
    // 2. height
    // 3. width (?)
    // 4. X
    // 5. Y
    // 6. Z

    if(sendMulticast) {

        struct AddFoodPacket {
            long    foodNum;
            float   foodHeight;
            float   foodX;
            float   foodY;
            float   foodZ;
        };

        AddFoodPacket * afp = new AddFoodPacket();
        afp->foodNum       = sendFood->Number();
        afp->foodHeight    = sendFood->gFoodHeight;
        afp->foodX         = sendFood->x();
        afp->foodY         = sendFood->y();
        afp->foodZ         = sendFood->z();

        QByteArray datagram;
        QDataStream out(&datagram, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << MSG_TYPE_FOOD_ADD
            << qint64(afp->foodNum)
            << afp->foodHeight
            << afp->foodX
            << afp->foodY
            << afp->foodZ;

        udpSocket->writeDatagram(datagram, groupAddress, 45454);

        delete(afp);

    }
}

/**
 * This will remove a food item from all clients.
 *
 * @brief MulticastSender::foodRemoveMsg
 * @param sendFood
 */
void MulticastSender::foodRemoveMsg(food * sendFood) {
    // On a food removal, send the following data so the food can be removed from the client
    // 1. foodNum
    if(sendMulticast) {

        struct FoodRemovePacket {
            long    foodNum;
        };

        FoodRemovePacket * frp = new FoodRemovePacket();
        frp->foodNum       = sendFood->Number();

        QByteArray datagram;
        QDataStream out(&datagram, QIODevice::WriteOnly);
        out.setVersion(QDataStream::Qt_4_3);
        out << MSG_TYPE_FOOD_REMOVE
            << qint64(frp->foodNum);

        udpSocket->writeDatagram(datagram, groupAddress, 45454);

        delete(frp);
    }
}


void MulticastSender::startSending()
{
    sendMulticast = true;
    startButton->setEnabled(false);
    //timer->start(1000);
}
