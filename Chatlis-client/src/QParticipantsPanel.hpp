#pragma once

#include <QScrollArea>
#include <QList>
#include "QParticipantLabel.hpp"

class QParticipantsPanel : public QScrollArea
{
	Q_OBJECT

public:
	QParticipantsPanel();

public slots:
	void addClient(qint64 clientId, QStringView clientName, QStringView clientComputerName);
	void removeClient(qint64 clientId);
	void clientNameChanged(qint64 clientId, QStringView clientName, QStringView clientComputerName);
	void clear();

private:
	auto findClientLabel(qint64 clientId) const
	{
		return std::ranges::find_if(mParticipants, [clientId](const QParticipantLabel* participant)
		{
			return participant->clientId() == clientId;
		});
	}

	QList<QParticipantLabel*> mParticipants;
};
