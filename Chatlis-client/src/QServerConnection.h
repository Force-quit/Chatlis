#pragma once

#include <QSslSocket>
#include <QString>
#include "QClientInfo.h"
#include <QSslKey>
#include <QSslCertificate>

class QServerConnection  : public QSslSocket
{
	Q_OBJECT

public:
	QServerConnection();
	~QServerConnection();

	void connectToServer(const QString& address, const QString& portNb, const QString& username, const QString& computerName);
	void changeName(const QString& name, const QString& computerName);
	void registration(const QString& name, const QString& computerName);

public slots:
	void sendNewChatMessage(const QString& message);

signals:
	void clearChatbox();
	void addMessageToChatbox(const QString username, const QString message);
	void newClient(qint64 clientId, QStringView username, QStringView computerName);
	void removeClient(qint64 clientId);
	void appendSystemMessage(const QString message);
	void appendServerMessage(const QString message);
	void clientChangedName(qint64 clientId, QStringView username, QStringView computerName);
	void serverDisconnected();

private slots:
	void receivedData();
	void notifyDisconnection();

private:
	QSslKey key;
	QSslCertificate cert;
};