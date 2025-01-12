#include "QParticipantsPanel.h"
#include <QStringList>
#include <QSizePolicy>
#include <QVBoxLayout>
#include "QParticipantLabel.hpp"
#include <algorithm>

QParticipantsPanel::QParticipantsPanel()
{
	auto* layout{ new QVBoxLayout };
	layout->setAlignment(Qt::AlignTop);
	setLayout(layout);
}

void QParticipantsPanel::addClient(qint64 clientId, QStringView clientName, QStringView clientComputerName)
{
	auto* participantLabel{ new QParticipantLabel(clientId, clientName, clientComputerName) };
	mParticipants.append(participantLabel);
	layout()->addWidget(participantLabel);
}

void QParticipantsPanel::removeClient(qint64 clientId)
{
	mParticipants.erase(findClientLabel(clientId));
	delete (*findClientLabel(clientId));
}

void QParticipantsPanel::clientNameChanged(qint64 clientId, QStringView clientName, QStringView clientComputerName)
{
	(*findClientLabel(clientId))->changeName(clientName, clientComputerName);
}

void QParticipantsPanel::clear()
{
	std::ranges::for_each(mParticipants, [](QParticipantLabel* participant)
	{
		delete participant;
	});

	mParticipants.clear();
}