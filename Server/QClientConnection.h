#pragma once

#include <QTcpSocket>

class QClientConnection : public QObject
{
	Q_OBJECT

public:
	QClientConnection(QObject* parent, QTcpSocket* socket);
	~QClientConnection();

	QString peerName() const;
	QString peerAddress() const;

signals:
	void messageReceived(QString message, QClientConnection* sender);
	void notifyDisconnect(QClientConnection* disconnectedClient);

private slots:
	void receivedData();
	void clientDisconnected();

private:
	QTcpSocket* socket;
};
