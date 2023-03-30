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

	QFile caFile("../../SSL/rootCA.pem");
	caFile.open(QIODevice::ReadOnly);
	QSslCertificate caCert = QSslCertificate(caFile.readAll());
	caFile.close();

	QSslConfiguration config;
	config.addCaCertificate(caCert);
	setSslConfiguration(config);

	//ingore bad hostname
	QList<QSslError> errorsToIgnore;
	const QString serverCertPath("../../SSL/client1.pem");
	auto serverCert = QSslCertificate::fromPath(serverCertPath);
	Q_ASSERT(!serverCert.isEmpty());
	errorsToIgnore << QSslError(QSslError::HostNameMismatch, serverCert.at(0));
	ignoreSslErrors(errorsToIgnore);

	connect(this, &QIODevice::readyRead, this, &QServerConnection::receivedData);
	connect(this, &QAbstractSocket::disconnected, this, &QServerConnection::notifyDisconnection);
}

QString QServerConnection::getUsername() const
{
	return client.getUsername();
}


void QServerConnection::connectToServer(const QString& address, const QString& portNb)
{
	if (!address.isEmpty()) {
		emit clearChatbox();
		connectToHostEncrypted(address, portNb.toUInt());
		if (waitForConnected()) {
			emit appendSystemMessage("Connected to " + peerAddress().toString());
			if (waitForEncrypted()) {
				emit appendSystemMessage("Encrypted connection established");
				QByteArray buffer;
				QDataStream dataStream(&buffer, QIODevice::WriteOnly);
				dataStream << NetworkMessage::Type::clientRegistration << client.getUsername() << client.getComputerName();

				QDataStream dataToSend(this);
				dataToSend << buffer;
			}
			else {
				emit appendSystemMessage("Encryption failed. Disconnecting");
				disconnectFromHost();
			}
		}
		else {
			emit appendSystemMessage("Connection failed");
		}
	}
}

void QServerConnection::sendNewChatMessage(const QString& message)
{
	QByteArray buffer;
	QDataStream dataStream(&buffer, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientSentMessage << message;
	QDataStream dataToSend(this);
	dataToSend << buffer;
}

void QServerConnection::changeUserName(const QString& newUsername)
{
	client.setUsername(newUsername);

	QByteArray buffer;
	QDataStream dataStream(&buffer, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientChangeUsername << newUsername;
	QDataStream dataToSend(this);
	dataToSend << buffer;
}

void QServerConnection::changeComputerName(const QString& newComputerName)
{
	client.setComputerName(newComputerName);

	QByteArray buffer;
	QDataStream dataStream(&buffer, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientChangeComputerName << newComputerName;
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

	QString previousUsername;
	QString previousComputerName;
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
		case NetworkMessage::Type::clientChangeUsername:
			processedData >> previousUsername;
			processedData >> computerName;
			processedData >> username;
			emit otherClientChangedUsername(previousUsername, computerName, username);
			emit appendServerMessage(previousUsername + '@' + computerName + " changed their name to " + username + '@' + computerName);
			break;
		case NetworkMessage::Type::clientChangeComputerName :
			processedData >> username;
			processedData >> previousComputerName;
			processedData >> computerName;
			emit otherClientChangedComputerName(username, previousComputerName, computerName);
			emit appendServerMessage(username + '@' + previousComputerName + " changed their name to " + username + '@' + computerName);
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
