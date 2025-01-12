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
	addPendingConnection(new QClientConnection(socketDescriptor));
}

void QChatlisServer::getNextPendingConnection()
{
	QClientConnection* newClient{ dynamic_cast<QClientConnection*>(nextPendingConnection()) };

	connect(newClient, &QClientConnection::registration, this, &QChatlisServer::replicateNewUser);
	connect(newClient, &QClientConnection::newClientMessage, this, &QChatlisServer::replicateClientMessage);
	connect(newClient, &QClientConnection::clientChangedName, this, &QChatlisServer::replicateClientChangedName);
	connect(newClient, &QAbstractSocket::disconnected, this, &QChatlisServer::clientDisconnected);
	
	if (!mConnectedClients.isEmpty())
	{
		newClient->replicateExistingClients(mConnectedClients);
	}
	
	mConnectedClients.append(newClient);

	emit serverLog("Client attempting to connect from " + newClient->clientAddress());
}

void QChatlisServer::replicateNewUser()
{
	QClientConnection* newClient{ dynamic_cast<QClientConnection*>(sender()) };
	
	QString log("Connection opened with client %1@%2 from %3");
	const QString& username(newClient->getClientUsername());
	const QString& computerName(newClient->getClientComputerName());
	QString peerAddress(QHostAddress(newClient->peerAddress().toIPv4Address()).toString());
	emit serverLog(log.arg(username, computerName, peerAddress));

	// Use QClientconnection local port to get a unique identifier for the localClient
	for (QClientConnection* localClient : mConnectedClients)
	{
		if (localClient != newClient)
		{
			localClient->replicateNewClient(newClient->socketDescriptor(), username, computerName);
		}
	}
}

void QChatlisServer::replicateClientChangedName()
{
	QClientConnection* initiatorClient{ dynamic_cast<QClientConnection*>(sender()) };

	std::ranges::for_each(mConnectedClients, [initiatorClient](QClientConnection* client)
	{
		if (client != initiatorClient)
		{
			client->replicateClientChangedName(initiatorClient->socketDescriptor(), initiatorClient->getClientUsername(), initiatorClient->getClientComputerName());
		}
	});
}

void QChatlisServer::replicateClientMessage(const QString message)
{
	QClientConnection* senderConnection{ dynamic_cast<QClientConnection*>(sender()) };
	const QString& username = senderConnection->getClientUsername();

	for (QClientConnection* localClient : mConnectedClients)
		if (localClient != senderConnection)
			localClient->replicateClientMessage(username, message);
}

void QChatlisServer::clientDisconnected()
{
	QClientConnection* disconnectedClient{ dynamic_cast<QClientConnection*>(sender()) };
	
	mConnectedClients.removeOne(disconnectedClient);
	disconnectedClient->deleteLater();

	if (disconnectedClient->wasEncrypted())
	{
		QString log("Connection closed with client %1@%2 from %3");
		emit serverLog(log.arg(disconnectedClient->getClientUsername(), disconnectedClient->getClientComputerName(), disconnectedClient->clientAddress()));

		std::ranges::for_each(mConnectedClients, [disconnectedClient](QClientConnection* client)
		{
			client->replicateDisconnect(disconnectedClient->socketDescriptor(), disconnectedClient->getClientUsername(), disconnectedClient->getClientComputerName());
		});
	}
	else
	{
		emit serverLog("Connection closed with unencrypted client at " + disconnectedClient->clientAddress());
	}
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
		emit serverLog("Server listening on port " + QString::number(QChatlisServer::PORT_NB) + '\n');
	}
	else
	{
		emit serverLog("No local ip address found.\n");
	}
}

void QChatlisServer::start()
{
	displayIpAddresses();
	listen(QHostAddress::Any, QChatlisServer::PORT_NB);
}