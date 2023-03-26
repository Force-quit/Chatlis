#include "QClientConnection.h"

QClientConnection::QClientConnection(QObject* parent, QTcpSocket* socket)
	: QObject(parent), socket{ socket }
{
	connect(socket, &QTcpSocket::readyRead, this, &QClientConnection::receivedData);
	connect(socket, &QTcpSocket::disconnected, this, &QClientConnection::clientDisconnected);
}

void QClientConnection::receivedData()
{
	emit messageReceived(socket->readAll(), this);
}

void QClientConnection::clientDisconnected()
{
	emit notifyDisconnect(this);
}

QClientConnection::~QClientConnection()
{
	socket->close();
	socket->deleteLater();
}

QString QClientConnection::peerName() const
{
	return peerAddress();
}

QString QClientConnection::peerAddress() const
{
	return socket->peerAddress().toString();
}

void QClientConnection::sendMessage(QString message, QClientConnection* sender)
{
	QString output = '[' + sender->peerName() + "] : " + message;

	socket->write(output.toUtf8());
}
