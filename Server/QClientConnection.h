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
	void replicateDisconnect(const QString& clientName, const QString& computerName);

	QString getClientUsername() const;
	QString getClientComputerName() const;

signals:
	void newClient();
	void newClientMessage(const QString message);

private slots:
	void receivedData();

private:
	void sendNetworkMessage(const QByteArray& toSend);

	QClientInfo client;
};
