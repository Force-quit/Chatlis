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
#include <QGroupBox>
#include "QServerConnection.h"
#include "QChatlisMenuBar.h"

QChatRoomMainWindow::QChatRoomMainWindow(QWidget* parent)
	: QMainWindow(parent), serverConnection{ new QServerConnection(this) }
{
	QWidget* centralWidget{ new QWidget };
	QVBoxLayout* centralLayout{ new QVBoxLayout };

	QChatlisMenuBar* topMenuBar{ new QChatlisMenuBar(this) };
	setMenuBar(topMenuBar);

	QSplitter* topLayout{ new QSplitter };
	topLayout->setChildrenCollapsible(false);
	QChatbox* chatbox{ new QChatbox(this) };
	QWidget* usersWidget{ new QWidget };
	QVBoxLayout* usersLayout{ new QVBoxLayout };
	QGroupBox* user{new QGroupBox("Your name") };
	QVBoxLayout* userLayout{ new QVBoxLayout };
	QLabel* userLabel{ new QLabel(serverConnection->getUsername() + '@' + serverConnection->getComputerName()) };
	userLayout->addWidget(userLabel);
	user->setLayout(userLayout);
	QGroupBox* participants{ new QGroupBox("Participants") };
	QVBoxLayout* participantsLayout{ new QVBoxLayout };
	QParticipantsPanel* participantsPanel{ new QParticipantsPanel };
	participantsLayout->addWidget(participantsPanel);
	participants->setLayout(participantsLayout);
	usersLayout->addWidget(user);
	usersLayout->addWidget(participants);
	usersWidget->setLayout(usersLayout);
	topLayout->addWidget(chatbox);
	topLayout->addWidget(usersWidget);

	QHBoxLayout* textInputLayout{ new QHBoxLayout };
	QLabel* textInputLabel{ new QLabel(tr("Message :")) };
	QLineEdit* textInput{ new QLineEdit };
	textInputLayout->addWidget(textInputLabel);
	textInputLayout->addWidget(textInput);

	centralLayout->addWidget(topLayout);
	centralLayout->addLayout(textInputLayout);
	centralWidget->setLayout(centralLayout);
	setCentralWidget(centralWidget);

	connect(topMenuBar, &QChatlisMenuBar::actionConnectToServer, this, &QChatRoomMainWindow::tryConnectToServer);
	connect(topMenuBar, &QChatlisMenuBar::actionDisconnectFromServer, this, &QChatRoomMainWindow::tryDisconnectFromServer);
	connect(topMenuBar, &QChatlisMenuBar::actionChangeUsername, this, &QChatRoomMainWindow::tryChangeUsername);
	connect(topMenuBar, &QChatlisMenuBar::actionChangeComputerName, this, &QChatRoomMainWindow::tryChangeComputerName);

	connect(textInput, &QLineEdit::returnPressed, [=]() {
		QString currentText(textInput->text().simplified());
		if (!currentText.isEmpty())
		{
			chatbox->appendUserMessage(serverConnection->getUsername(), currentText);
			serverConnection->sendNewChatMessage(currentText);
		}
		else
			chatbox->appendSystemMessage("Can't send empty messages -_-");
		
		textInput->clear();
	});

	connect(serverConnection, &QServerConnection::clearChatbox, chatbox, &QChatbox::clearChat);
	connect(serverConnection, &QServerConnection::appendSystemMessage, chatbox, &QChatbox::appendSystemMessage);
	connect(serverConnection, &QServerConnection::addMessageToChatbox, chatbox, &QChatbox::appendUserMessage);
	connect(serverConnection, &QServerConnection::appendServerMessage, chatbox, &QChatbox::appendServerMessage);

	connect(serverConnection, &QServerConnection::newClient, participantsPanel, &QParticipantsPanel::addParticipant);
	connect(serverConnection, &QServerConnection::serverDisconnected, participantsPanel, &QParticipantsPanel::clear);
	connect(serverConnection, &QServerConnection::clientChangedUsername, this, [=](const QString& newUsername) {
		userLabel->setText(newUsername + '@' + serverConnection->getComputerName());
	});
	connect(serverConnection, &QServerConnection::clientChangedComputerName, this, [=](const QString& newComputerName) {
		userLabel->setText(serverConnection->getUsername() + '@' + newComputerName);
	});
	connect(serverConnection, &QServerConnection::otherClientChangedUsername, participantsPanel, &QParticipantsPanel::otherClientChangedUsername);
	connect(serverConnection, &QServerConnection::otherClientChangedComputerName, participantsPanel, &QParticipantsPanel::otherClientChangedComputerName);


	connect(serverConnection, &QServerConnection::removeClient, participantsPanel, &QParticipantsPanel::removeParticipant);

	resize(650, 350);
	setCentralWidget(centralWidget);
	setWindowTitle("Chatlis");
	setWindowIcon(QIcon("group-chat.png"));
}

void QChatRoomMainWindow::tryConnectToServer()
{
	QString serverAddress(QInputDialog::getText(this,
		tr("Chatlis server connection"),
		tr("Server address (ip:port)")));
	if (!serverAddress.isEmpty())
	{
		QStringList ipAndPort = serverAddress.split(':');
		if (ipAndPort.size() == 2 && !ipAndPort[0].isEmpty() && !ipAndPort[1].isEmpty())
			serverConnection->connectToServer(ipAndPort[0], ipAndPort[1]);
		else
			QMessageBox::critical(this, "Wrong format", "Address and port of Chatlis server should be XXX.XXX.XXX.XXX:PPPPP");
	}
}

void QChatRoomMainWindow::tryDisconnectFromServer()
{
	serverConnection->disconnectFromHost();
}

void QChatRoomMainWindow::tryChangeUsername()
{
	QString newUsername(QInputDialog::getText(this,
		tr("Change username"),
		tr("Username to display")));
	if (!newUsername.isEmpty())
		serverConnection->changeUserName(newUsername);
}

void QChatRoomMainWindow::tryChangeComputerName()
{
	QString newComputerName(QInputDialog::getText(this,
		tr("Change computer name"),
		tr("Computer name to display")));
	if (!newComputerName.isEmpty())
		serverConnection->changeComputerName(newComputerName);
}

QChatRoomMainWindow::~QChatRoomMainWindow()
{
	delete serverConnection;
}
