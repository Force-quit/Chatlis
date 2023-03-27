#include "QParticipantsPanel.h"
#include <QStringList>
#include <QSizePolicy>

QParticipantsPanel::QParticipantsPanel(QWidget* parent)
	: QListView(parent), model{ new QStringListModel }
{
	QSizePolicy modifiedPolicy{ sizePolicy() };
	modifiedPolicy.setHorizontalStretch(1);
	setSizePolicy(modifiedPolicy);

	setModel(model);
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	setFocusPolicy(Qt::NoFocus);
}

void QParticipantsPanel::addParticipant(const QString participantName, const QString participantComputerName)
{
	QStringList currentList{ model->stringList() };
	currentList.append(participantName + '@' + participantComputerName);
	model->setStringList(currentList);
}

void QParticipantsPanel::removeParticipant(const QString& participantToRemove)
{
	QStringList currentList{ model->stringList() };
	currentList.removeOne(participantToRemove);
	model->setStringList(currentList);
}

QParticipantsPanel::~QParticipantsPanel() {}