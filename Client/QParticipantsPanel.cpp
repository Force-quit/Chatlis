#include "QParticipantsPanel.h"
#include <QStringList>
#include <QSizePolicy>
#include <QStringListModel>

QParticipantsPanel::QParticipantsPanel(QWidget* parent)
	: QListView(parent), participantsList{ new QStringList }
{
	QSizePolicy modifiedPolicy{ sizePolicy() };
	modifiedPolicy.setHorizontalStretch(1);
	setSizePolicy(modifiedPolicy);

	QStringListModel* model{ new QStringListModel(*participantsList, this) };
	setModel(model);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setFocusPolicy(Qt::NoFocus);
}

void QParticipantsPanel::addParticipant(const QString participantName, const QString participantComputerName)
{
	participantsList->append(participantName + '@' + participantComputerName);
	emit addedParticipant(participantName);
}

void QParticipantsPanel::removeParticipant(const QString participantName, const QString participantComputerName)
{
	participantsList->removeOne(participantName + '@' + participantComputerName);
	emit removedParticipant(participantName);
}

void QParticipantsPanel::otherClientChangedUsername(const QString previousUsername, const QString computerName, const QString newUsername)
{
	for (auto& element : *participantsList)
	{
		if (element.contains(previousUsername + '@' + computerName))
		{
			element = newUsername + '@' + computerName;
			break;
		}
	}
}

void QParticipantsPanel::otherClientChangedComputerName(const QString username, const QString previousComputerName, const QString newComputerName)
{
	for (auto& element : *participantsList)
	{
		if (element.contains(username + '@' + previousComputerName))
		{
			element = username + '@' + newComputerName;
			break;
		}
	}
}

void QParticipantsPanel::clear()
{
	participantsList->clear();
}

QParticipantsPanel::~QParticipantsPanel() {}