#pragma once

#include <QSslSocket>
#include <QString>
#include "../../Common/QClientInfo.h"
#include <QSslKey>
#include <QSslCertificate>

class QServerConnection  : public QSslSocket
{
	Q_OBJECT

public:
	QServerConnection(QObject *parent);
	~QServerConnection();

	void connectToServer(const QString& address, const QString& portNb, const QString& username, const QString& computerName);
	void changeUserName(const QString& newUsername);
	void changeComputerName(const QString& newComputerName);

public slots:
	void sendNewChatMessage(const QString& message);

signals:
	void clearChatbox();
	void addMessageToChatbox(const QString username, const QString message);
	void newClient(const QString username, const QString computerName);
	void removeClient(const QString username, const QString computerName);
	void appendSystemMessage(const QString message);
	void appendServerMessage(const QString message);
	void otherClientChangedUsername(const QString previousUsername, const QString computerName, const QString newUsername);
	void otherClientChangedComputerName(const QString username, const QString previousComputerName, const QString newComputerName);
	void serverDisconnected();

private slots:
	void receivedData();
	void notifyDisconnection();

private:
	QSslKey key;
	QSslCertificate cert;
};