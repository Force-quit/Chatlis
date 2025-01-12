#pragma once

#include <QMainWindow>
#include "QChatWidget.h"
#include <QMenuBar>
#include <QMenu>
#include "QServerConnection.h"
#include "QParticipantsPanel.h"
#include <QLabel>
#include <QVBoxLayout>
#include "QClientInfo.h"

class QChatRoomMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	QChatRoomMainWindow(QWidget *parent = nullptr);

private slots:
	void actionConnectToServer();
	void actionDisconnectFromServer();
	void actionChangeUsername();
	void actionChangeComputerName();

private:
	QClientInfo localClient;
	QServerConnection serverConnection;

	QLabel* userDisplayName;
	QParticipantsPanel* participantsPanel;
	QChatWidget* chatWidget;
	QWidget* initParticipantsWidget();
};
