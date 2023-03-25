#pragma once

#include <QListView>
#include <QStringListModel>


class QParticipantsPanel : public QListView
{
	Q_OBJECT

public:
	QParticipantsPanel(QWidget *parent = nullptr);
	~QParticipantsPanel();

public slots:
	void addParticipant(const QString& newParticipant);
	void removeParticipant(const QString& participantToRemove);

private:
	QStringListModel* model;
};
