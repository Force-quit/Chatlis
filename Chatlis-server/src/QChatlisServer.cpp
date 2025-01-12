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
	connect(newClient, &QClientConnection::clientChangedName, this, &QChatlisServer::replicateClientChangedName);
	connect(newClient, &QAbstractSocket::disconnected, this, &QChatlisServer::clientDisconnected);

	if (mConnectedClients.size() > 0)
	{
		QList<std::tuple<qint64, QString, QString>> existingClients;
		std::ranges::transform(mConnectedClients, std::back_inserter(existingClients), [&existingClients](const QClientConnection* localClient)
		{
			return std::tuple{localClient->socketDescriptor(), localClient->getClientUsername(), localClient->getClientComputerName() };
		});
		newClient->replicateExistingClients(existingClients);
	}

	mConnectedClients.append(newClient);
}

void QChatlisServer::replicateNewUser()
{
	QClientConnection* newClient{ dynamic_cast<QClientConnection*>(sender()) };
	
	QString log("Log : connection opened with client %1@%2 from %3");
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
	
	const QString& username = disconnectedClient->getClientUsername();
	const QString& computerName = disconnectedClient->getClientComputerName();

	QString log("Log : connection closed with client [%1] (%2)");
	QHostAddress formated(disconnectedClient->peerAddress().toIPv4Address());
	emit serverLog(log.arg(username, formated.toString()));

	std::ranges::for_each(mConnectedClients, [disconnectedClient](QClientConnection* client)
	{
		if (client != disconnectedClient)
		{
			client->replicateDisconnect(disconnectedClient->socketDescriptor(), disconnectedClient->getClientUsername(), disconnectedClient->getClientComputerName());
		}
	});

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