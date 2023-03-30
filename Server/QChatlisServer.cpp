#include "QChatlisServer.h"
#include <QtNetwork>
#include <QVector>
#include "../QClientInfo.h"
#include <QPair>
#include <QFile>
#include <QSslKey>
#include <QSslCertificate>
#include <QSslConfiguration>

const quint16 QChatlisServer::PORT_NB{ 59532 };

QChatlisServer::QChatlisServer(QObject* parent)
	: QSslServer(parent), connectedClients()
{
	connect(this, &QTcpServer::pendingConnectionAvailable, this, &QChatlisServer::getNextPendingConnection);
	listen(QHostAddress::Any, QChatlisServer::PORT_NB);
}

void QChatlisServer::incomingConnection(qintptr socketDescriptor)
{
	QFile keyFile("../../SSL/client1.key");
	keyFile.open(QIODevice::ReadOnly);
	QSslKey privateKey = QSslKey(keyFile.readAll(), QSsl::Rsa);
	keyFile.close();

	QFile certFile("../../SSL/client1.pem");
	certFile.open(QIODevice::ReadOnly);
	QSslCertificate localCert = QSslCertificate(certFile.readAll());
	certFile.close();

	QClientConnection* newClient{ new QClientConnection(this) };
	newClient->setSocketDescriptor(socketDescriptor);
	newClient->setPrivateKey(privateKey);
	newClient->setLocalCertificate(localCert);
	newClient->startServerEncryption();
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
			existingClients.push_back(QPair<QString, QString>(client->getClientUsername(), client->getClientComputerName()));
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
	emit serverLog(log.arg(username, senderConnection->peerAddress().toString()));

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
	const QString& username = disconnectedClient->getClientUsername();
	const QString& computerName = disconnectedClient->getClientComputerName();

	QString log("Log : connection closed with client %2");
	emit serverLog(log.arg(disconnectedClient->peerAddress().toString()));

	for (QClientConnection* client : connectedClients)
		if (client != disconnectedClient)
			client->replicateDisconnect(username, computerName);

	connectedClients.removeOne(disconnectedClient);
	disconnectedClient->deleteLater();
}

QChatlisServer::~QChatlisServer() {}