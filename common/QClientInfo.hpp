#pragma once

#include <QObject>
#include <QString>

class QClientInfo  : public QObject
{
	Q_OBJECT

public:
	QClientInfo();

	[[nodiscard]] QString getUsername() const;
	[[nodiscard]] QString getComputerName() const;

	void setUsername(const QString& newName);
	void setComputerName(const QString& newComputerName);

private:
	QString username{ qEnvironmentVariable("USERNAME") };
	QString computerName{ qEnvironmentVariable("USERDOMAIN") };
};