#pragma once

#include <QObject>
#include <QString>
#include <QHostAddress>

class QClient  : public QObject
{
	Q_OBJECT

public:
	QClient(QObject *parent);
	~QClient();

	QString nickName() const;

	void setUsername(const QString& newName);
	void setComputerName(const QString& newComputerName);

private:
	QString username;
	QString computerName;
};