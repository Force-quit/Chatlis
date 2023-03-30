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
	void addParticipant(const QString participantName, const QString participantComputerName);
	void removeParticipant(const QString participantName, const QString participantComputerName);
	void otherClientChangedUsername(const QString previousUsername, const QString computerName, const QString newUsername);
	void otherClientChangedComputerName(const QString username, const QString previousComputerName, const QString newComputerName);
	void clear();

private:
	QStringListModel* model;
};
