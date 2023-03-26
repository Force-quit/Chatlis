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

	void connectToServer(const QString& address, const QString& portNb);

private slots:
	void shareClientInfo();
	void receivedData();

private:
	QClientInfo client;
};