#include "QChatlisServer.h"
#include <QtNetwork>
#include <QTcpSocket>
#include <QVector>

const quint16 QChatlisServer::PORT_NB{ 59532 };

QChatlisServer::QChatlisServer(QObject* parent)
	: QTcpServer(parent), connectedClients()
{
	connect(this, &QTcpServer::pendingConnectionAvailable, this, &QChatlisServer::incomingConnection);
	listen(QHostAddress::Any, QChatlisServer::PORT_NB);
	currentListeningPort = serverPort();
}

void QChatlisServer::incomingConnection()
{
	QTcpSocket* incomingNewConnection{ nextPendingConnection() };
	QClientConnection* newClient{ new QClientConnection(this, incomingNewConnection) };

	connectedClients.insert(newClient->peerName(), newClient);
	emit newOutput("Log : connection opened with client [" + newClient->peerName() + ']');
	connect(newClient, &QClientConnection::messageReceived, this, &QChatlisServer::messageReceived);
	connect(newClient, &QClientConnection::notifyDisconnect, this, &QChatlisServer::clientDisconnected);
}

QChatlisServer::~QChatlisServer()
{
	for (auto i : connectedClients)
		delete i;
}

quint16 QChatlisServer::listeningPort() const
{
	return currentListeningPort;
}

void QChatlisServer::messageReceived(QString message, QClientConnection* sender)
{
	emit newOutput("[" + sender->peerName() + "] : " + message);
	for (auto i : connectedClients)
		if (i != sender)
			newOutput("Log: sent message to [" + i->peerName() + "]");
			//i->sendMessage(message);
}

void QChatlisServer::clientDisconnected(QClientConnection* disconnectedClient)
{
	emit newOutput("Log : connection closed with client [" + disconnectedClient->peerName() + ']');
	connectedClients.remove(disconnectedClient->peerName());
	delete disconnectedClient;
}
