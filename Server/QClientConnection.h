#pragma once

#include <QTcpSocket>

class QClientConnection  : public QObject
{
	Q_OBJECT

public:
	QClientConnection(QObject *parent, QTcpSocket* socket);
	~QClientConnection();

	QString peerName() const;
	QString peerAddress() const;

signals:
	void messageReceived(QString message);

private slots:
	void receivedData();

private:
	QTcpSocket* socket;
};
