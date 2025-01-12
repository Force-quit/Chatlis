#pragma once

#include <QSslSocket>
#include "QClientInfo.h"
#include <QSpan>

class QClientConnection : public QSslSocket
{
	Q_OBJECT

public:
	QClientConnection(qintptr socketDescriptor);

	void replicateExistingClients(QSpan<QClientConnection*> existingClients);
	void replicateClientMessage(const QString& clientName, const QString& message);
	void replicateNewClient(qint64 clientId, const QString& clientName, const QString& computerName);
	void replicateDisconnect(qint64 clientId, const QString& clientName, const QString& computerName);
	void replicateClientChangedName(qint64 clientId, const QString& clientName, const QString& clientComputerName);

	QString getClientUsername() const;
	QString getClientComputerName() const;

	[[nodiscard]] bool wasEncrypted() const { return hasBeenEncrypted; }
	[[nodiscard]] QString clientAddress() const { return mClientAddress; }

signals:
	void registration();
	void newClientMessage(const QString message);
	void clientChangedName();
  
public slots:
	void receivedData();

private:
	void sendNetworkMessage(const QByteArray& toSend);
	QClientInfo mClientInfo;
	QString mClientAddress;
	bool hasBeenEncrypted{};
};
