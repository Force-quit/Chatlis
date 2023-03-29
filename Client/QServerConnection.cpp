#include "QServerConnection.h"
#include <QHostAddress>
#include <QDataStream>
#include <QByteArray>
#include <QThread>
#include "../NetworkMessage.h"
#include <QFile>
#include <QSslKey>
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QTimer>

QServerConnection::QServerConnection(QObject* parent)
	: QSslSocket(parent), client(this)
{
	QFile keyFile("../../SSL/blue_local.key");
	keyFile.open(QIODevice::ReadOnly);
	QSslKey privateKey = QSslKey(keyFile.readAll(), QSsl::Rsa);
	keyFile.close();

	QFile certFile("../../SSL/blue_local.pem");
	certFile.open(QIODevice::ReadOnly);
	QSslCertificate localCert = QSslCertificate(certFile.readAll());
	certFile.close();

	QFile caFile("../../SSL/red_ca.pem");
	caFile.open(QIODevice::ReadOnly);
	QSslCertificate caCert = QSslCertificate(caFile.readAll());
	caFile.close();

	QSslConfiguration config;
	config.addCaCertificate(caCert);
	config.setLocalCertificate(localCert);
	config.setPeerVerifyMode(QSslSocket::VerifyPeer);
	config.setPrivateKey(privateKey);
	config.setProtocol(QSsl::TlsV1_3OrLater);
	setSslConfiguration(config);

	connect(this, &QSslSocket::encrypted, this, &QServerConnection::shareClientInfo);
	connect(this, &QIODevice::readyRead, this, &QServerConnection::receivedData);
	connect(this, &QAbstractSocket::disconnected, this, &QServerConnection::notifyDisconnection);
}

QString QServerConnection::getUsername() const
{
	return client.getUsername();
}


void QServerConnection::connectToServer(const QString& address, const QString& portNb)
{
	if (!address.isEmpty())
		connectToHostEncrypted(address, portNb.toUInt());
}

void QServerConnection::shareClientInfo()
{
	emit clearChatbox();
	emit appendSystemMessage("Connected to " + peerAddress().toString());

	QByteArray buffer;
	QDataStream dataStream(&buffer, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientRegistration << client.getUsername() << client.getComputerName();

	QDataStream dataToSend(this);
	dataToSend << buffer;
}

void QServerConnection::sendNewChatMessage(const QString& message)
{
	QByteArray buffer;
	QDataStream dataStream(&buffer, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientSentMessage << message;
	QDataStream dataToSend(this);
	dataToSend << buffer;
}

void QServerConnection::receivedData()
{
	QByteArray buffer;
	QDataStream dataStream(this);
	dataStream >> buffer;
	QDataStream processedData(buffer);

	NetworkMessage::Type messageType{};
	processedData >> messageType;

	QString username;
	QString computerName;
	QString message;

	switch (messageType)
	{
	case NetworkMessage::Type::clientSentMessage:
		processedData >> username;
		processedData >> message;
		emit addMessageToChatbox(username, message);
		break;
	case NetworkMessage::Type::replicateExistingClients:
		while (!processedData.atEnd())
		{
			processedData >> username;
			processedData >> computerName;
			emit newClient(username, computerName);
		}
		break;
	case NetworkMessage::Type::clientAdded:
		processedData >> username;
		processedData >> computerName;
		emit appendServerMessage(username + " has joined the server");
		emit newClient(username, computerName);
		break;
	case NetworkMessage::Type::clientDisconnected:
		processedData >> username;
		processedData >> computerName;
		emit removeClient(username, computerName);
		emit appendServerMessage(username + " has left the server");
		break;
	default:
		break;
	}
}

void QServerConnection::notifyDisconnection()
{
	emit appendSystemMessage("Disconnected from server");
	emit serverDisconnected();
}

QServerConnection::~QServerConnection() 
{
	disconnect(this, &QAbstractSocket::disconnected, this, &QServerConnection::notifyDisconnection);
}