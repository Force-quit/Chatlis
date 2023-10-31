#pragma once

#include <QObject>
#include <QString>
#include <QHostAddress>

class QClientInfo : public QObject
{
	Q_OBJECT

public:
	QClientInfo(bool isLocalClient);

	~QClientInfo();

	[[nodiscard]] QString getUsername() const;
	[[nodiscard]] QString getComputerName() const;

	void setUsername(const QString& newName);
	void setComputerName(const QString& newComputerName);

private:
	QString username;
	QString computerName;
};