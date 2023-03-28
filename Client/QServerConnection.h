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

signals:
	void clearChatbox();
	void addMessageToChatbox(const QString username, const QString message);
	void newClient(const QString username, const QString computerName);
	void removeClient(const QString username, const QString computerName);
	void appendSystemMessage(const QString message);
	void appendServerMessage(const QString message);
	void serverDisconnected();

private slots:	
	void shareClientInfo();
	void receivedData();
	void notifyDisconnection();

private:
	QClientInfo client;
};