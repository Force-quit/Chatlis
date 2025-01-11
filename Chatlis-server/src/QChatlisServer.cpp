#include "QChatlisServer.hpp"
#include <QtNetwork>
#include "QClientInfo.h"
#include <QPair>
#include <algorithm>

QChatlisServer::QChatlisServer()
{
	connect(this, &QTcpServer::pendingConnectionAvailable, this, &QChatlisServer::getNextPendingConnection);
}

void QChatlisServer::incomingConnection(qintptr socketDescriptor)
{
	addPendingConnection(new QClientConnection(this, socketDescriptor));
}

void QChatlisServer::getNextPendingConnection()
{
	QClientConnection* newClient{ dynamic_cast<QClientConnection*>(nextPendingConnection()) };

	connect(newClient, &QClientConnection::newClient, this, &QChatlisServer::replicateNewUser);
	connect(newClient, &QClientConnection::newClientMessage, this, &QChatlisServer::replicateClientMessage);
	connect(newClient, &QClientConnection::newClientName, this, &QChatlisServer::replicateClientNewUsername);
	connect(newClient, &QClientConnection::newClientComputerName, this, &QChatlisServer::replicateClientNewComputerName);
	connect(newClient, &QAbstractSocket::disconnected, this, &QChatlisServer::clientDisconnected);

	if (mConnectedClients.size() > 0)
	{
		QList<QPair<QString, QString>> existingClients;
		std::ranges::transform(mConnectedClients, std::back_inserter(existingClients), [&existingClients](const QClientConnection* client)
		{
			return std::pair{ client->getClientUsername(), client->getClientComputerName() };
		});
		newClient->replicateExistingClients(existingClients);
	}

	mConnectedClients.append(newClient);
}

void QChatlisServer::replicateNewUser()
{
	QClientConnection* newClient{ dynamic_cast<QClientConnection*>(sender()) };
	
	QString log("Log : connection opened with client %1@%2 %3:%4 at local port %5");
	const QString& username(newClient->getClientUsername());
	const QString& computerName(newClient->getClientComputerName());
	QString peerAddress(QHostAddress(newClient->peerAddress().toIPv4Address()).toString());
	QString peerPort(newClient->peerPort());
	QString localPort(newClient->localPort());
	emit serverLog(log.arg(username, computerName, peerAddress, peerPort, localPort));

	// Use QClientconnection local port to get a unique identifier for the client
	for (QClientConnection* client : mConnectedClients)
	{
		if (client != newClient)
		{
			client->replicateNewClient(username, computerName);
		}
	}
}

void QChatlisServer::replicateClientNewUsername(const QString previousUsername)
{
	QClientConnection* senderConnection{ dynamic_cast<QClientConnection*>(sender()) };

	for (QClientConnection* client : mConnectedClients)
		if (client != senderConnection)
			client->replicateClientNewUsername(previousUsername, senderConnection->getClientComputerName(), senderConnection->getClientUsername());
}

void QChatlisServer::replicateClientNewComputerName(const QString previousComputerName)
{
	QClientConnection* senderConnection{ dynamic_cast<QClientConnection*>(sender()) };

	for (QClientConnection* client : mConnectedClients)
		if (client != senderConnection)
			client->replicateClientNewComputerName(senderConnection->getClientUsername(), previousComputerName, senderConnection->getClientComputerName());
}

void QChatlisServer::replicateClientMessage(const QString message)
{
	QClientConnection* senderConnection{ dynamic_cast<QClientConnection*>(sender()) };
	const QString& username = senderConnection->getClientUsername();

	for (QClientConnection* client : mConnectedClients)
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

		for (QClientConnection* client : mConnectedClients)
			if (client != disconnectedClient)
				client->replicateDisconnect(username, computerName);
	}

	mConnectedClients.removeOne(disconnectedClient);
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