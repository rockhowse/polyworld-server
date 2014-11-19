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
void Server::sendStep(int simStep, float sceneRotation){
    if(client) {
        char buffer[1024] = {0};

        strcpy(buffer, "Derp");

        qint64 bytes_sent = client->write(buffer, 1024);

        qint64 derp = 27;
   }
}

// closes the client connection
void Server::closeClient() {
    if(client) {
        client->close();
    }
}
