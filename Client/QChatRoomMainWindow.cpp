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

	connect(topMenuBar, &QChatlisMenuBar::actionConnectToServer, this, &QChatRoomMainWindow::tryConnectToServer);
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
		if (ipAndPort.size() == 2)
			serverConnection->connectToServer(ipAndPort[0], ipAndPort[1]);
		else
			QMessageBox::critical(this, "Wrong format", "Address and port of Chatlis server should be XXX.XXX.XXX.XXX:PPPPP");
	}
}

void QChatRoomMainWindow::tryChangeUsername()
{
	QString newUsername(QInputDialog::getText(this,
		tr("Change username"),
		tr("Username to display")));
	if (!newUsername.isEmpty())
	{
		QStringList ipAndPort = newUsername.split(':');
		if (ipAndPort.size() == 2)
			serverConnection->connectToServer(ipAndPort[0], ipAndPort[1]);
		else
			QMessageBox::critical(this, "Wrong format", "Address and port of Chatlis server should be XXX.XXX.XXX.XXX:PPPPP");
	}
}

void QChatRoomMainWindow::tryChangeComputerName()
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

QChatRoomMainWindow::~QChatRoomMainWindow()
{
	delete serverConnection;
}