#pragma once

#include <QMainWindow>
#include "QChatbox.h"
#include <QMenuBar>
#include <QMenu>
#include "QServerConnection.h"

class QChatRoomMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	QChatRoomMainWindow(QWidget *parent = nullptr);
	~QChatRoomMainWindow();

private slots:
	void tryConnectToServer();
	void tryChangeUsername();

private:
	QServerConnection* serverConnection;
};
