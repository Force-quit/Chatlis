#pragma once

#include <QTcpSocket>
#include <QString>
#include "../QClient.h"

class QServerConnection  : public QTcpSocket
{
	Q_OBJECT

public:
	QServerConnection(QObject *parent);
	~QServerConnection();

	void connectToServer(const QString& address, const QString& portNb);

signals:
	void writeToOutput(const QString& toWrite);

private slots:
	void connectedToServer();

private:
	QClient client;
};