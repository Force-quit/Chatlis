#pragma once

#include <QTcpSocket>
#include <QString>
#include "../QClientInfo.h"

class QServerConnection  : public QTcpSocket
{
	Q_OBJECT

public:
	QServerConnection(QObject *parent);
	~QServerConnection();

	QString getUsername() const;

	void connectToServer(const QString& address, const QString& portNb);
	void sendNewChatMessage(const QString& message);
	void changeUserName(const QString& newUsername);
	void changeComputerName(const QString& newComputerName);


signals:
	void clearChatbox();
	void addMessageToChatbox(const QString username, const QString message);
	void newClient(const QString username, const QString computerName);
	void removeClient(const QString username, const QString computerName);
	void appendSystemMessage(const QString message);
	void appendServerMessage(const QString message);
	void serverDisconnected();
	void otherClientChangedUsername(const QString previousUsername, const QString computerName, const QString newUsername);
	void otherClientChangedComputerName(const QString username, const QString previousComputerName, const QString newComputerName);

private slots:	
	void shareClientInfo();
	void receivedData();
	void notifyDisconnection();

private:
	QClientInfo client;
};