// server.cc
#include "Server.h"
#include <iostream>
using namespace std;

Server::Server(QObject* parent): QObject(parent)
{
  connect(&server, SIGNAL(newConnection()),
    this, SLOT(acceptConnection()));

  server.listen(QHostAddress::Any, 8888);
}

Server::~Server()
{
  server.close();
}

void Server::acceptConnection()
{
  client = server.nextPendingConnection();

  connect(client, SIGNAL(readyRead()),
    this, SLOT(startRead()));
}

void Server::startRead()
{
  char buffer[1024] = {0};
  client->read(buffer, client->bytesAvailable());
  printf("%s", buffer);
  //cout >> buffer >> endl;
  //client->close();
}

// sends a step message to the client
void Server::simStepMsg(int curStep, float sceneRotation){
    if(client) {
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
        // 2. food's X length
        // 3. food's Y length
        // 4. food's Z length

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
                << ssh->foodCount
                << ssh->sceneRotation;

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

            // send the data
            udpSocket->writeDatagram(datagram, groupAddress, 45454);

            delete(sad);
            delete(sfd);
            delete(ssh);
        }
        /*
        char buffer[1024] = {0};

        strcpy(buffer, "Derp");

        qint64 bytes_sent = client->write(buffer, 1024);

        qint64 derp = 27;
        */
   }
}

// closes the client connection
void Server::closeClient() {
    if(client) {
        client->close();
    }
}
