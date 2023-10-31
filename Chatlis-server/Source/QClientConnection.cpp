#include "../Headers/QClientConnection.h"
#include "../../Common/NetworkMessage.h"
#include <QString>
#include <QFile>
#include <QSslKey>
#include <QSslCertificate>

QClientConnection::QClientConnection(QObject* parent, qintptr socketDescriptor)
	: QSslSocket(parent), client(false), isEncrypted{false}
{
	
	QFile keyFile("SSL/server.key");
	keyFile.open(QIODevice::ReadOnly);
	QSslKey privateKey = QSslKey(keyFile.readAll(), QSsl::Rsa);
	keyFile.close();

	QFile certFile("SSL/server.pem");
	certFile.open(QIODevice::ReadOnly);
	QSslCertificate localCert = QSslCertificate(certFile.readAll());
	certFile.close();

	setPrivateKey(privateKey);
	setLocalCertificate(localCert);
	setSocketDescriptor(socketDescriptor);
	startServerEncryption();

	connect(this, &QSslSocket::encrypted, this, &QClientConnection::onEncrypted);
	connect(this, &QClientConnection::readyRead, this, &QClientConnection::receivedData);
}

void QClientConnection::replicateExistingClients(const QList<QPair<QString, QString>>& existingClients)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::replicateExistingClients;

	for (auto& clientInfo : existingClients)
		dataStream << clientInfo.first << clientInfo.second;

	sendNetworkMessage(byteArray);
}

void QClientConnection::replicateClientMessage(const QString& clientName, const QString& message)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientSentMessage << clientName << message;

	sendNetworkMessage(byteArray);
}

void QClientConnection::replicateNewClient(const QString& clientName, const QString& computerName)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientAdded << clientName << computerName;

	sendNetworkMessage(byteArray);
}

void QClientConnection::replicateDisconnect(const QString& clientName, const QString& computerName)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientDisconnected << clientName << computerName;

	sendNetworkMessage(byteArray);
}

void QClientConnection::replicateClientNewUsername(const QString previousUsername, const QString computerName, const QString newUsername)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientChangeUsername << previousUsername << computerName << newUsername;

	sendNetworkMessage(byteArray);
}

void QClientConnection::replicateClientNewComputerName(const QString username, const QString previousComputerName, const QString newComputerName)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientChangeComputerName << username << previousComputerName << newComputerName;

	sendNetworkMessage(byteArray);
}

QString QClientConnection::getClientUsername() const
{
	return client.getUsername();
}

QString QClientConnection::getClientComputerName() const
{
	return client.getComputerName();
}

bool QClientConnection::getEncrypted() const
{
	return isEncrypted;
}

void QClientConnection::receivedData()
{
	QByteArray buffer;
	QDataStream dataStream(this);
	dataStream >> buffer;
	QDataStream processedData(buffer);

	NetworkMessage::Type messageType{};
	processedData >> messageType;

	QString previousComputerName(client.getComputerName());
	QString previousUsername(client.getUsername());

	QString username;
	QString computerName;
	QString clientMessage;


	switch (messageType)
	{
	case NetworkMessage::Type::invalidType:
		break;
	case NetworkMessage::Type::clientRegistration:
		processedData >> username;
		processedData >> computerName;
		client.setUsername(username);
		client.setComputerName(computerName);
		emit newClient();
		break;
	case NetworkMessage::Type::clientChangeUsername:
		processedData >> username;
		client.setUsername(username);
		emit newClientName(previousUsername);
		break;
	case NetworkMessage::Type::clientChangeComputerName:
		processedData >> computerName;
		client.setComputerName(computerName);
		emit newClientComputerName(previousComputerName);
		break;
	case NetworkMessage::Type::clientSentMessage:
		processedData >> clientMessage;
		emit newClientMessage(clientMessage);
		break;
	default:
		break;
	}
}

void QClientConnection::onEncrypted()
{
	isEncrypted = true;
}


void QClientConnection::sendNetworkMessage(const QByteArray& toSend)
{
	QDataStream dataStream(this);
	dataStream << toSend;
}
