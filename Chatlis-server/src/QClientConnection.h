#pragma once

#include <QSslSocket>
#include "QClientInfo.h"
#include <QList>
#include <tuple>

class QClientConnection : public QSslSocket
{
	Q_OBJECT

public:
	QClientConnection(QObject* parent, qintptr socketDescriptor);

	void replicateExistingClients(const QList<std::tuple<qint64, QString, QString>>& existingClients);
	void replicateClientMessage(const QString& clientName, const QString& message);
	void replicateNewClient(qint64 clientId, const QString& clientName, const QString& computerName);
	void replicateDisconnect(qint64 clientId, const QString& clientName, const QString& computerName);
	void replicateClientChangedName(qint64 clientId, const QString& clientName, const QString& clientComputerName);

	QString getClientUsername() const;
	QString getClientComputerName() const;

signals:
	void newClient();
	void newClientMessage(const QString message);
	void clientChangedName();
  
public slots:
	void receivedData();

private:
	void sendNetworkMessage(const QByteArray& toSend);

	QClientInfo client;
};
