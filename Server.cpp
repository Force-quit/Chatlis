#include "Server.h"
#include <QtNetwork>
#include "Connection.h"

Server::Server(QObject* parent)
    : QTcpServer(parent)
{
    listen();
}

void Server::incomingConnection(qintptr socketDescriptor)
{
    Connection* connection{ new Connection(socketDescriptor, this) };
    emit newConnection(connection);
}