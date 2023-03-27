#include "QChatRoomMainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>
#include "QChatbox.h"
#include <QTimer>
#include <QIcon>
#include "QParticipantsPanel.h"
#include <QLineEdit>
#include <QMenuBar>
#include <QAction>
#include <QKeySequence>
#include <QStringList>
#include <QInputDialog>
#include <QMessageBox>
#include "QServerConnection.h"



QChatRoomMainWindow::QChatRoomMainWindow(QWidget* parent)
	: QMainWindow(parent), serverConnection{ new QServerConnection(this) }
{
	QWidget* centralWidget{ new QWidget };
	QVBoxLayout* centralLayout{ new QVBoxLayout };

	QMenuBar* topMenuBar{ initMenuBar() };
	setMenuBar(topMenuBar);

	QSplitter* topLayout{ new QSplitter };
	topLayout->setChildrenCollapsible(false);
	QChatbox* chatbox{ new QChatbox(this) };
	QParticipantsPanel* participantsPanel{ new QParticipantsPanel };
	topLayout->addWidget(chatbox);
	topLayout->addWidget(participantsPanel);

	QHBoxLayout* textInputLayout{ new QHBoxLayout };
	QLabel* textInputLabel{ new QLabel(tr("Message :")) };
	QLineEdit* textInput{ new QLineEdit };
	textInputLayout->addWidget(textInputLabel);
	textInputLayout->addWidget(textInput);
	
	centralLayout->addWidget(topLayout);
	centralLayout->addLayout(textInputLayout);
	centralWidget->setLayout(centralLayout);
	setCentralWidget(centralWidget);

	connect(textInput, &QLineEdit::returnPressed, [=]() {chatbox->appendMessage(serverConnection->getUsername(), textInput->text()); });
	connect(textInput, &QLineEdit::returnPressed, textInput, &QLineEdit::clear);
	connect(serverConnection, &QAbstractSocket::connected, [=]() {
		chatbox->clearChat(); 
		chatbox->appendMessage("Server", "Connected to " + serverConnection->peerAddress().toString()); });
	connect(serverConnection, &QServerConnection::addClientToPanel, participantsPanel, &QParticipantsPanel::addParticipant);
	connect(participantsPanel, &QParticipantsPanel::addedParticipant, chatbox, &QChatbox::participantJoined);
	connect(serverConnection, &QServerConnection::removeClientFromPanel, participantsPanel, &QParticipantsPanel::removeParticipant);
	connect(participantsPanel, &QParticipantsPanel::removedParticipant, chatbox, &QChatbox::participantLeft);
	connect(serverConnection, &QServerConnection::addMessageToChatbox, chatbox, &QChatbox::appendMessage);


	resize(650, 350);
	setCentralWidget(centralWidget);
	setWindowTitle("Chatlis");
	setWindowIcon(QIcon("group-chat.png"));
}



QMenuBar* QChatRoomMainWindow::initMenuBar()
{
	QMenuBar* menuBar{ new QMenuBar };

	/*auto* settingsMenu{ new QMenu("Settings") };
	auto* changeUsernameAction{ new QAction("Change username") };
	changeUsernameAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
	connect(connectAction, &QAction::triggered, this, &QChatRoomMainWindow::tryConnectToServer);*/


	auto* connectAction{ new QAction("Connect to server", menuBar) };
	connectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_C));
	connect(connectAction, &QAction::triggered, this, &QChatRoomMainWindow::tryConnectToServer);
	menuBar->addAction(connectAction);

	return menuBar;
}

void QChatRoomMainWindow::tryConnectToServer()
{
	QString serverAddress(QInputDialog::getText(this,
		tr("Chatlis server connection"),
		tr("Server address (ip:port)")));
	if (!serverAddress.isEmpty())
	{
		QStringList ipAndPort = serverAddress.split(':');
		if (ipAndPort.size() == 2)
			serverConnection->connectToServer(ipAndPort[0], ipAndPort[1]);
		else
			QMessageBox::critical(this, "Wrong format", "Address and port of Chatlis server should be XXX.XXX.XXX.XXX:PPPPP");
	}
}

void QChatRoomMainWindow::tryChangeUsername()
{
	/*QString newUsername(QInputDialog::getText(this,
		tr("Change your username"),
		tr("New username")));
	if (!newUsername.isEmpty())
		serverConnection.connectToServer(newUsername);*/
}

QChatRoomMainWindow::~QChatRoomMainWindow() 
{
	delete serverConnection;
}