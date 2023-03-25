#include "QChatRoomMainWindow.h"
#include <QVBoxLayout>
#include "QChatbox.h"
#include <QTimer>
#include <QIcon>

QChatRoomMainWindow::QChatRoomMainWindow(QWidget *parent)
	: QMainWindow(parent), chatbox{ new QChatbox }
{
	resize(650, 350);
	setCentralWidget(chatbox);
	setWindowTitle("Chatlis");
	setWindowIcon(QIcon("group-chat.png"));
}

QChatRoomMainWindow::~QChatRoomMainWindow() {}