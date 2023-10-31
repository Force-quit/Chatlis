#pragma once

#include <QSslSocket>
#include "../../Common/QClientInfo.h"

class QClientConnection : public QSslSocket
{
	Q_OBJECT

public:
	QClientConnection(QObject* parent, qintptr socketDescriptor);

	void replicateExistingClients(const QList<QPair<QString, QString>>& existingClients);
	void replicateClientMessage(const QString& clientName, const QString& message);
	void replicateNewClient(const QString& clientName, const QString& computerName);
	void replicateDisconnect(const QString& clientName, const QString& computerName);
	void replicateClientNewUsername(const QString previousUsername, const QString computerName, const QString newUsername);
	void replicateClientNewComputerName(const QString username, const QString previousComputerName, const QString newComputerName);

	[[nodiscard]] QString getClientUsername() const;
	[[nodiscard]] QString getClientComputerName() const;
	[[nodiscard]] bool getEncrypted() const;
	void setEncrypted(bool encrypted);

signals:
	void newClient();
	void newClientMessage(const QString message);
	void newClientName(const QString previousUsername);
	void newClientComputerName(const QString previousComputerName);
  
public slots:
	void receivedData();
	void onEncrypted();

private:
	void sendNetworkMessage(const QByteArray& toSend);
	bool isEncrypted;
	QClientInfo client;
};
