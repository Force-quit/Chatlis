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

signals:
	void addMessageToChatbox(const QString& username, const QString& message);
	void addClientToPanel(const QString username, const QString computerName);
	void removeClientFromPanel(const QString username, const QString computerName);

private slots:
	void shareClientInfo();
	void receivedData();

private:
	QClientInfo client;
};