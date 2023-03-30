#include "QParticipantsPanel.h"
#include <QStringList>
#include <QSizePolicy>

QParticipantsPanel::QParticipantsPanel(QWidget* parent)
	: QListView(parent), model{}
{
	QSizePolicy modifiedPolicy{ sizePolicy() };
	modifiedPolicy.setHorizontalStretch(1);
	setSizePolicy(modifiedPolicy);

	model = new QStringListModel(this);
	setModel(model);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setFocusPolicy(Qt::NoFocus);
}

void QParticipantsPanel::addParticipant(const QString participantName, const QString participantComputerName)
{
	QStringList participantsList(model->stringList());
	participantsList.append(participantName + '@' + participantComputerName);
	model->setStringList(participantsList);
}

void QParticipantsPanel::removeParticipant(const QString participantName, const QString participantComputerName)
{
	QStringList participantsList(model->stringList());

	participantsList.removeOne(participantName + '@' + participantComputerName);
	model->setStringList(participantsList);
}

void QParticipantsPanel::otherClientChangedUsername(const QString previousUsername, const QString computerName, const QString newUsername)
{
	QStringList participantsList(model->stringList());

	for (auto& element : participantsList)
	{
		if (element.contains(previousUsername + '@' + computerName))
		{
			element = newUsername + '@' + computerName;
			break;
		}
	}
	model->setStringList(participantsList);
}

void QParticipantsPanel::otherClientChangedComputerName(const QString username, const QString previousComputerName, const QString newComputerName)
{
	QStringList participantsList(model->stringList());

	for (auto& element : participantsList)
	{
		if (element.contains(username + '@' + previousComputerName))
		{
			element = username + '@' + newComputerName;
			break;
		}
	}
	model->setStringList(participantsList);
}

void QParticipantsPanel::clear()
{
	QStringList participantsList(model->stringList());
	participantsList.clear();
	model->setStringList(participantsList);
}

QParticipantsPanel::~QParticipantsPanel() {}