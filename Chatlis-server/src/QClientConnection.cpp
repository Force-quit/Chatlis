#include "QClientConnection.h"
#include "NetworkMessage.h"
#include <QString>
#include <QFile>
#include <QSslKey>
#include <QSslCertificate>
#include <tuple>
#include <QList>

QClientConnection::QClientConnection(QObject* parent, qintptr socketDescriptor)
	: QSslSocket(parent), client(false)
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

	connect(this, &QClientConnection::readyRead, this, &QClientConnection::receivedData);
}

void QClientConnection::replicateExistingClients(const QList<std::tuple<qint64, QString, QString>>& existingClients)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::replicateExistingClients;

	for (auto& clientInfo : existingClients)
	{
		qint64 clientId;
		QString clientName;
		QString computerName;
		std::tie(clientId, clientName, computerName) = clientInfo;
		dataStream << clientId << clientName << computerName;
	}

	sendNetworkMessage(byteArray);
}

void QClientConnection::replicateClientMessage(const QString& clientName, const QString& message)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientSentMessage << clientName << message;

	sendNetworkMessage(byteArray);
}

void QClientConnection::replicateNewClient(qint64 clientId, const QString& clientName, const QString& computerName)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientAdded << clientId << clientName << computerName;

	sendNetworkMessage(byteArray);
}

void QClientConnection::replicateDisconnect(qint64 clientId, const QString& clientName, const QString& computerName)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientDisconnected << clientId << clientName << computerName;
	sendNetworkMessage(byteArray);
}

void QClientConnection::replicateClientChangedName(qint64 clientId, const QString& clientName, const QString& clientComputerName)
{
	QByteArray byteArray;
	QDataStream dataStream(&byteArray, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientChangedName << clientId << clientName << clientComputerName;

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
	case NetworkMessage::Type::clientChangedName:
		processedData >> username;
		processedData >> computerName;
		client.setUsername(username);
		client.setComputerName(computerName);
		emit clientChangedName();
		break;
	case NetworkMessage::Type::clientSentMessage:
		processedData >> clientMessage;
		emit newClientMessage(clientMessage);
		break;
	default:
		break;
	}
}

void QClientConnection::sendNetworkMessage(const QByteArray& toSend)
{
	QDataStream dataStream(this);
	dataStream << toSend;
}