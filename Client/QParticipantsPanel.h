#pragma once

#include <QListView>
#include <QStringListModel>


class QParticipantsPanel : public QListView
{
	Q_OBJECT

public:
	QParticipantsPanel(QWidget *parent = nullptr);
	~QParticipantsPanel();

signals:
	void addedParticipant(const QString participantName);
	void removedParticipant(const QString participantName);

public slots:
	void addParticipant(const QString participantName, const QString participantComputerName);
	void removeParticipant(const QString& participantName, const QString participantComputerName);

private:
	QStringListModel* model;
};
