#pragma once

#include <QLabel>
#include <QString>
#include <QDebug>

class QParticipantLabel : public QLabel
{
	Q_OBJECT
public:
	QParticipantLabel(qint64 clientId, QStringView clientName, QStringView clientComputerName)
		: mClientId{ clientId }
	{
		changeName(clientName, clientComputerName);
	}

	void changeName(QStringView clientName, QStringView clientComputerName)
	{
		setText(QString("%1@%2 (%3)").arg(clientName, clientComputerName, QString::number(mClientId)));

	}

	[[nodiscard]] inline qint64 clientId() const
	{
		return mClientId;
	}

private:
	qint64 mClientId;
};