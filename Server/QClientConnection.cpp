#include "QClientConnection.h"

QClientConnection::QClientConnection(QObject* parent, QTcpSocket* socket)
	: QObject(parent), socket{ socket }
{
	connect(socket, &QTcpSocket::readyRead, this, &QClientConnection::receivedData);
}

void QClientConnection::receivedData()
{
	emit messageReceived(socket->readAll());
}

QClientConnection::~QClientConnection()
{
	socket->write("Ok bye now\n");
	socket->waitForBytesWritten();
	socket->close();
	delete socket;
}

QString QClientConnection::peerName() const
{
	return peerAddress();
}

QString QClientConnection::peerAddress() const
{
	return socket->peerAddress().toString();
}
