#include "../Headers/QChatlisServer-CLI.h"
#include <QtNetwork>
#include <QVector>
#include "../../Common/QClientInfo.h"
#include <QPair>
#include <QSslConfiguration>

const quint16 QChatlisServer::PORT_NB{ 59532 };

QChatlisServer::QChatlisServer()
	: connectedClients(), cout(stdout)
{
	connect(this, &QTcpServer::pendingConnectionAvailable, this, &QChatlisServer::getNextPendingConnection);
	listen(QHostAddress::Any, QChatlisServer::PORT_NB);
}

void QChatlisServer::serverLog(const QString& message)
{
	cout << message << '\n';
	cout.flush();
}

void QChatlisServer::incomingConnection(qintptr socketDescriptor)
{
	QClientConnection* newClient{ new QClientConnection(this, socketDescriptor) };
	addPendingConnection(newClient);
}

void QChatlisServer::getNextPendingConnection()
{
	QClientConnection* newClient{ dynamic_cast<QClientConnection*>(nextPendingConnection()) };
	connect(newClient, &QClientConnection::newClient, this, &QChatlisServer::replicateNewUser);
	connect(newClient, &QClientConnection::newClientMessage, this, &QChatlisServer::replicateClientMessage);
	connect(newClient, &QClientConnection::newClientName, this, &QChatlisServer::replicateClientNewUsername);
	connect(newClient, &QClientConnection::newClientComputerName, this, &QChatlisServer::replicateClientNewComputerName);
	connect(newClient, &QAbstractSocket::disconnected, this, &QChatlisServer::clientDisconnected);

	if (connectedClients.size() > 0)
	{
		QList<QPair<QString, QString>> existingClients;
		for (QClientConnection* client : connectedClients)
			existingClients.emplace_back(client->getClientUsername(), client->getClientComputerName());
		newClient->replicateExistingClients(existingClients);
	}

	connectedClients.append(newClient);
}

void QChatlisServer::replicateNewUser()
{
	QClientConnection* senderConnection{ dynamic_cast<QClientConnection*>(sender()) };
	const QString username(senderConnection->getClientUsername());
	const QString computerName(senderConnection->getClientComputerName());


	QString log("Log : connection opened with client [%1] (%2)");
	QHostAddress formated(senderConnection->peerAddress().toIPv4Address());
	serverLog(log.arg(username, formated.toString()));

	for (QClientConnection* client : connectedClients)
		if (client != senderConnection)
			client->replicateNewClient(username, computerName);
}

void QChatlisServer::replicateClientNewUsername(const QString previousUsername)
{
	QClientConnection* senderConnection{ dynamic_cast<QClientConnection*>(sender()) };

	for (QClientConnection* client : connectedClients)
		if (client != senderConnection)
			client->replicateClientNewUsername(previousUsername, senderConnection->getClientComputerName(), senderConnection->getClientUsername());
}

void QChatlisServer::replicateClientNewComputerName(const QString previousComputerName)
{
	QClientConnection* senderConnection{ dynamic_cast<QClientConnection*>(sender()) };

	for (QClientConnection* client : connectedClients)
		if (client != senderConnection)
			client->replicateClientNewComputerName(senderConnection->getClientUsername(), previousComputerName, senderConnection->getClientComputerName());
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
	
	if (disconnectedClient->getEncrypted())
	{
		const QString& username = disconnectedClient->getClientUsername();
		const QString& computerName = disconnectedClient->getClientComputerName();

		QString log("Log : connection closed with client [%1] (%2)");
		QHostAddress formated(disconnectedClient->peerAddress().toIPv4Address());
		serverLog(log.arg(username, formated.toString()));

		for (QClientConnection* client : connectedClients)
			if (client != disconnectedClient)
				client->replicateDisconnect(username, computerName);
	}

	connectedClients.removeOne(disconnectedClient);
	disconnectedClient->deleteLater();
}

QChatlisServer::~QChatlisServer() {}