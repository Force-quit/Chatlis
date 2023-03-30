#pragma once

#include <QListView>

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
	void removeParticipant(const QString participantName, const QString participantComputerName);
	void otherClientChangedUsername(const QString previousUsername, const QString computerName, const QString newUsername);
	void otherClientChangedComputerName(const QString username, const QString previousComputerName, const QString newComputerName);
	void clear();

private:
	QStringList* participantsList;
};
