#include "QServerConnection.h"
#include <QHostAddress>

QServerConnection::QServerConnection(QObject* parent)
	: QTcpSocket(parent), client(this)
{
	connect(this, &QAbstractSocket::connected, this, &QServerConnection::connectedToServer);
}

void QServerConnection::connectToServer(const QString& address, const QString& portNb)
{
	QHostAddress temp(address);
	if (!temp.isNull())
		connectToHost(temp, portNb.toUInt());
}

void QServerConnection::connectedToServer()
{
	//
}

QServerConnection::~QServerConnection() {}