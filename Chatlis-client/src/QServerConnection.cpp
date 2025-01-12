#include "QServerConnection.h"
#include <QHostAddress>
#include <QDataStream>
#include <QByteArray>
#include "NetworkMessage.h"
#include <QFile>
#include <QSslKey>
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QTimer>

QServerConnection::QServerConnection()
{
	auto caCerts = QSslCertificate::fromPath("SSL/ca/*.pem", QSsl::Pem, QSslCertificate::PatternSyntax::Wildcard);
	Q_ASSERT(!caCerts.isEmpty());
	QSslConfiguration config;
	config.setCaCertificates(caCerts);
	setSslConfiguration(config);

	auto serverCerts = QSslCertificate::fromPath("SSL/public/*.pem", QSsl::Pem, QSslCertificate::PatternSyntax::Wildcard);
	Q_ASSERT(!serverCerts.isEmpty());
	QList<QSslError> errorsToIgnore;
	for (const auto& cert : serverCerts)
		errorsToIgnore << QSslError(QSslError::HostNameMismatch, cert);
	ignoreSslErrors(errorsToIgnore);

	connect(this, &QIODevice::readyRead, this, &QServerConnection::receivedData);
	connect(this, &QAbstractSocket::disconnected, this, &QServerConnection::notifyDisconnection);
}

void QServerConnection::connectToServer(const QString& address, const QString& portNb, const QString& username, const QString& computerName)
{
	if (address.isEmpty() || portNb.isEmpty())
	{
		return;
	}

	if (isEncrypted())
	{
		disconnectFromHost();
		waitForDisconnected(5000);
	}

	connectToHostEncrypted(address, portNb.toUInt());
	emit appendSystemMessage("Connecting to " + address + ':' + portNb);
	if (waitForConnected(10000)) 
	{
		emit appendSystemMessage("Connected to " + address + ':' + portNb);
		emit appendSystemMessage("Trying to set up encryption");
		if (waitForEncrypted(10000)) 
		{
			emit appendSystemMessage("Encrypted connection established");
			registration(username, computerName);
		}
		else 
		{
			emit appendSystemMessage("Encryption failed. Disconnecting");
			disconnectFromHost();
		}
	}
	else
	{
		emit appendSystemMessage("Connection failed");
	}
}

void QServerConnection::sendNewChatMessage(const QString& message)
{
	if (isEncrypted())
	{
		QByteArray buffer;
		QDataStream dataStream(&buffer, QIODevice::WriteOnly);
		dataStream << NetworkMessage::Type::clientSentMessage << message;
		QDataStream dataToSend(this);
		dataToSend << buffer;
	}
}

void QServerConnection::changeName(const QString& name, const QString& computerName)
{
	if (isEncrypted())
	{
		QByteArray buffer;
		QDataStream dataStream(&buffer, QIODevice::WriteOnly);
		dataStream << NetworkMessage::Type::clientChangedName << name << computerName;
		QDataStream dataToSend(this);
		dataToSend << buffer;
	}
}

void QServerConnection::registration(const QString& name, const QString& computerName)
{
	QByteArray buffer;
	QDataStream dataStream(&buffer, QIODevice::WriteOnly);
	dataStream << NetworkMessage::Type::clientRegistration << name << computerName;
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
	QString name;
	QString computerName;
	QString message;
	qint64 clientId;
	QString formattedMessage;

	switch (messageType)
	{
		case NetworkMessage::Type::clientSentMessage:
			processedData >> name;
			processedData >> message;
			emit addMessageToChatbox(name, message);
			break;
		case NetworkMessage::Type::replicateExistingClients:
			while (!processedData.atEnd())
			{
				processedData >> clientId;
				processedData >> name;
				processedData >> computerName;
				emit newClient(clientId, name, computerName);
			}
			break;
		case NetworkMessage::Type::clientAdded:
			processedData >> clientId;
			processedData >> name;
			processedData >> computerName;
			emit appendServerMessage(name + " has joined the server");
			emit newClient(clientId, name, computerName);
			break;
		case NetworkMessage::Type::clientChangedName:
			processedData >> clientId;
			processedData >> name;
			processedData >> computerName;
			emit clientChangedName(clientId, name, computerName);
			formattedMessage = QString("Client %1 changed their name to %2@%3").arg(QString::number(clientId), name, computerName);
			emit appendServerMessage(formattedMessage);
			break;
		case NetworkMessage::Type::clientDisconnected:
			processedData >> clientId;
			processedData >> name;
			processedData >> computerName;
			emit removeClient(clientId);
			emit appendServerMessage(name + " has left the server");
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
