#pragma once

#include <QSslSocket>
#include "../QClientInfo.h"

class QClientConnection : public QSslSocket
{
	Q_OBJECT

public:
	QClientConnection(QObject* parent);
	~QClientConnection();

	void replicateExistingClients(const QList<QPair<QString, QString>>& existingClients);
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
