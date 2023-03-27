#include "QChatlisServer.h"
#include <QtNetwork>
#include <QTcpSocket>
#include <QVector>


const quint16 QChatlisServer::PORT_NB{ 59532 };

QChatlisServer::QChatlisServer(QObject* parent)
	: QTcpServer(parent), connectedClients()
{
	listen(QHostAddress::Any, QChatlisServer::PORT_NB);

}

void QChatlisServer::incomingConnection(qintptr socketDescriptor)
{
	QClientConnection* newClient{ new QClientConnection(this) };
	newClient->setSocketDescriptor(socketDescriptor);
	addPendingConnection(newClient);
	emit newConnection();

	connect(newClient, &QClientConnection::newClient, this, &QChatlisServer::replicateNewUser);
	connect(newClient, &QAbstractSocket::disconnected, this, &QChatlisServer::clientDisconnected);

	for (QClientConnection* client : connectedClients)
		if (client != newClient)
			newClient->replicateNewClient(client->getClientUsername(), client->getClientComputerName());
}

void QChatlisServer::replicateNewUser()
{
	QClientConnection* senderConnection{ dynamic_cast<QClientConnection*>(sender()) };
	const QString username(senderConnection->getClientUsername());
	const QString computerName(senderConnection->getClientComputerName());


	QString log("Log : connection opened with client [%1] (%2)");
	emit serverLog(log.arg(username, senderConnection->peerAddress().toString()));

	for (QClientConnection* client : connectedClients)
		if (client != senderConnection)
			client->replicateNewClient(username, computerName);
}

void QChatlisServer::replicateClientMessage(const QString message)
{
	QClientConnection* senderConnection{ dynamic_cast<QClientConnection*>(sender()) };
	const QString& username = senderConnection->getClientUsername();

	for (QClientConnection* client : connectedClients)
		if (client != senderConnection)
			client->replicateClientMessage(username, message);
}

void QChatlisServer::clientDisconnected()
{
	QClientConnection* disconnectedClient{ dynamic_cast<QClientConnection*>(sender()) };

	QString log("Log : connection closed with client %2");
	emit serverLog(log.arg(disconnectedClient->peerAddress().toString()));

	connectedClients.removeOne(disconnectedClient);
	disconnectedClient->deleteLater();
}

QChatlisServer::~QChatlisServer()
{
	for (QClientConnection* client : connectedClients)
		client->abort();
}