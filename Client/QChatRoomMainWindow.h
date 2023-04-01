#pragma once

#include <QMainWindow>
#include "QChatWidget.h"
#include <QMenuBar>
#include <QMenu>
#include "QServerConnection.h"
#include "QParticipantsPanel.h"
#include <QLabel>
#include <QVBoxLayout>

class QChatRoomMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	QChatRoomMainWindow(QWidget *parent = nullptr);
	~QChatRoomMainWindow();

private slots:
	void actionConnectToServer();
	void actionDisconnectFromServer();
	void actionChangeUsername();
	void actionChangeComputerName();

private:
	QClientInfo client;

	QServerConnection* serverConnection;
	QLabel* userDisplayName;
	QParticipantsPanel* participantsPanel;
	QChatWidget* chatWidget;
	QWidget* initParticipantsWidget();
};
