#pragma once

#include <QMainWindow>
#include "QChatbox.h"

class QChatRoomMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	QChatRoomMainWindow(QWidget *parent = nullptr);
	~QChatRoomMainWindow();

private:
	QChatbox* chatbox;
};
