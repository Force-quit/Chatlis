#include "QServerConnection.h"
#include <QHostAddress>
#include <QDataStream>
#include <QByteArray>
#include "../NetworkMessage.h"

QServerConnection::QServerConnection(QObject* parent)
	: QTcpSocket(parent), client(this)
{
	connect(this, &QAbstractSocket::connected, this, &QServerConnection::shareClientInfo);
	connect(this, &QIODevice::readyRead, this, &QServerConnection::receivedData);
}

QString QServerConnection::getUsername() const
{
	return client.getUsername();
}


void QServerConnection::connectToServer(const QString& address, const QString& portNb)
{
	QHostAddress temp(address);
	if (!temp.isNull())
		connectToHost(temp, portNb.toUInt());
}

void QServerConnection::shareClientInfo()
{
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
	case NetworkMessage::Type::clientAdded:
		processedData >> username;
		processedData >> computerName;
		emit addClientToPanel(username, computerName);
		break;
	case NetworkMessage::Type::clientDisconnected:
		processedData >> username;
		processedData >> computerName;
		emit removeClientFromPanel(username, computerName);
		break;
	default:
		break;
	}
}

QServerConnection::~QServerConnection() {}