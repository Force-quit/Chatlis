#include "QChatlisServer.hpp"
#include <QtNetwork>
#include <QVector>
#include "QClientInfo.h"
#include <QPair>
#include <QSslConfiguration>

QChatlisServer::QChatlisServer()
{
	connect(this, &QTcpServer::pendingConnectionAvailable, this, &QChatlisServer::getNextPendingConnection);
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
	emit serverLog(log.arg(username, formated.toString()));

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
	
	if (disconnectedClient->isEncrypted())
	{
		const QString& username = disconnectedClient->getClientUsername();
		const QString& computerName = disconnectedClient->getClientComputerName();

		QString log("Log : connection closed with client [%1] (%2)");
		QHostAddress formated(disconnectedClient->peerAddress().toIPv4Address());
		emit serverLog(log.arg(username, formated.toString()));

		for (QClientConnection* client : connectedClients)
			if (client != disconnectedClient)
				client->replicateDisconnect(username, computerName);
	}

	connectedClients.removeOne(disconnectedClient);
	disconnectedClient->deleteLater();
}

void QChatlisServer::displayIpAddresses()
{
	QString IPV4Addresses;
	bool isFirstItem{ true };
	std::ranges::for_each(QNetworkInterface::allAddresses(), [&IPV4Addresses, &isFirstItem](const QHostAddress& address)
	{
		if (address.protocol() == QHostAddress::NetworkLayerProtocol::IPv4Protocol)
		{
			if (isFirstItem)
			{
				IPV4Addresses += QString("%1").arg(address.toString());
			}
			else
			{
				IPV4Addresses += QString(" | %1").arg(address.toString());
			}

			isFirstItem = false;
		}
	});

	if (!IPV4Addresses.isEmpty())
	{
		emit serverLog("Your local ip address(es) : " + IPV4Addresses);
		emit serverLog("Log : server listening on port " + QString::number(QChatlisServer::PORT_NB));
	}
	else
	{
		emit serverLog("No local ip address found.");
	}
}

void QChatlisServer::start()
{
	displayIpAddresses();
	listen(QHostAddress::Any, QChatlisServer::PORT_NB);
}