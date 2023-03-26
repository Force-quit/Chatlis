#pragma once

#include <QTcpSocket>
#include "../QClientInfo.h"

class QClientConnection : public QTcpSocket
{
	Q_OBJECT

public:
	QClientConnection(QObject* parent);
	~QClientConnection();

	void replicateClientMessage(const QString& clientName, const QString& message);
	void replicateNewClient(const QString& clientName, const QString& computerName);

signals:
	void newClient(const QString username, const QString computerName);
	void newClientMessage(const QString clientName, const QString message);

private slots:
	void receivedData();

private:
	void sendNetworkMessage(const QByteArray& toSend);

	QClientInfo client;
};
