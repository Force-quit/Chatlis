#include "QChatRoomMainWindow.h"
#include <QVBoxLayout>
#include "ChatDialog.h"

QChatRoomMainWindow::QChatRoomMainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	setCentralWidget(new ChatDialog);
}

QChatRoomMainWindow::~QChatRoomMainWindow() {}