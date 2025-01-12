#pragma once

#include <QObject>
#include <QString>
#include <QHostAddress>

class QClientInfo  : public QObject
{
	Q_OBJECT

public:
	QClientInfo(bool isLocalClient);

	[[nodiscard]] QString getUsername() const;
	[[nodiscard]] QString getComputerName() const;
	[[nodiscard]] qint64 getClientId() const;

	void setUsername(const QString& newName);
	void setComputerName(const QString& newComputerName);
	void setClientId(qint64 newClientId);

private:
	qint64 clientId;
	QString username;
	QString computerName;
};