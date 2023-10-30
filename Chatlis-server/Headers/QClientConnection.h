#pragma once

#include <QSslSocket>
#include "../QClientInfo.h"

class QClientConnection : public QSslSocket
{
	Q_OBJECT

public:
	QClientConnection(QObject* parent, qintptr socketDescriptor);
	~QClientConnection();

	void replicateExistingClients(const QList<QPair<QString, QString>>& existingClients);
	void replicateClientMessage(const QString& clientName, const QString& message);
	void replicateNewClient(const QString& clientName, const QString& computerName);
	void replicateDisconnect(const QString& clientName, const QString& computerName);
	void replicateClientNewUsername(const QString previousUsername, const QString computerName, const QString newUsername);
	void replicateClientNewComputerName(const QString username, const QString previousComputerName, const QString newComputerName);

	QString getClientUsername() const;
	QString getClientComputerName() const;

signals:
	void newClient();
	void newClientMessage(const QString message);
	void newClientName(const QString previousUsername);
	void newClientComputerName(const QString previousComputerName);
  
public slots:
	void receivedData();

private:
	void sendNetworkMessage(const QByteArray& toSend);

	QClientInfo client;
};
