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
#include "QChatWidget.h"

QChatRoomMainWindow::QChatRoomMainWindow(QWidget* parent)
	: QMainWindow(parent), client(true), serverConnection{ new QServerConnection(this) }, 
	userDisplayName{}, participantsPanel{}, chatWidget{ new QChatWidget(this, client) }
{
	QChatlisMenuBar* topMenuBar{ new QChatlisMenuBar(this) };
	setMenuBar(topMenuBar);

	QSplitter* splitter{ new QSplitter(this) };
	splitter->setChildrenCollapsible(false);

	splitter->addWidget(chatWidget);
	splitter->addWidget(initParticipantsWidget());

	resize(1000, 400);
	setCentralWidget(splitter);
	setWindowTitle("Chatlis");
	setWindowIcon(QIcon("group-chat.png"));

	connect(topMenuBar, &QChatlisMenuBar::actionConnectToServer, this, &QChatRoomMainWindow::actionConnectToServer);
	connect(topMenuBar, &QChatlisMenuBar::actionDisconnectFromServer, this, &QChatRoomMainWindow::actionDisconnectFromServer);
	connect(topMenuBar, &QChatlisMenuBar::actionChangeUsername, this, &QChatRoomMainWindow::actionChangeUsername);
	connect(topMenuBar, &QChatlisMenuBar::actionChangeComputerName, this, &QChatRoomMainWindow::actionChangeComputerName);

	connect(serverConnection, &QServerConnection::appendSystemMessage, chatWidget, &QChatWidget::appendSystemMessage);
	connect(serverConnection, &QServerConnection::addMessageToChatbox, chatWidget, &QChatWidget::appendUserMessage);
	connect(serverConnection, &QServerConnection::appendServerMessage, chatWidget, &QChatWidget::appendServerMessage);
	connect(chatWidget, &QChatWidget::sendMessage, serverConnection, &QServerConnection::sendNewChatMessage);


	connect(serverConnection, &QServerConnection::newClient, participantsPanel, &QParticipantsPanel::addParticipant);
	connect(serverConnection, &QServerConnection::serverDisconnected, participantsPanel, &QParticipantsPanel::clear);
	connect(serverConnection, &QServerConnection::otherClientChangedUsername, participantsPanel, &QParticipantsPanel::otherClientChangedUsername);
	connect(serverConnection, &QServerConnection::otherClientChangedComputerName, participantsPanel, &QParticipantsPanel::otherClientChangedComputerName);
	connect(serverConnection, &QServerConnection::removeClient, participantsPanel, &QParticipantsPanel::removeParticipant);

}

void QChatRoomMainWindow::actionConnectToServer()
{
	QString serverAddress(QInputDialog::getText(this, tr("Chatlis server connection"),	tr("Server address (ip:port)")));
	if (!serverAddress.isEmpty())
	{
		QStringList ipAndPort = serverAddress.split(':');
		if (ipAndPort.size() == 2 && !ipAndPort[0].isEmpty() && !ipAndPort[1].isEmpty())
		{
			chatWidget->clearMessages();
			serverConnection->connectToServer(ipAndPort[0], ipAndPort[1], client.getUsername(), client.getComputerName());
		}
		else
			QMessageBox::critical(this, "Wrong format", "Address and port of Chatlis server should be XXX.XXX.XXX.XXX:PPPPP");
	}
}

void QChatRoomMainWindow::actionDisconnectFromServer()
{
	serverConnection->disconnectFromHost();
}

void QChatRoomMainWindow::actionChangeUsername()
{
	QString newUsername(QInputDialog::getText(this,	tr("Change username"), tr("Username to display")));
	if (!newUsername.isEmpty())
	{
		if (newUsername.size() > 20)
			QMessageBox::critical(this, "Username too long", "Displayed username can't be longer than 20");
		else
		{
			client.setUsername(newUsername);
			serverConnection->changeUserName(newUsername);
			userDisplayName->setText(newUsername + '@' + client.getComputerName());
		}
	}
}

void QChatRoomMainWindow::actionChangeComputerName()
{
	QString newComputerName(QInputDialog::getText(this, tr("Change computer name"), tr("Computer name to display")));
	if (!newComputerName.isEmpty())
	{
		if (newComputerName.size() > 20)
			QMessageBox::critical(this, "Computer name too long", "Displayed computer name can't be longer than 20");
		else
		{
			client.setComputerName(newComputerName);
			serverConnection->changeComputerName(newComputerName);
			userDisplayName->setText(client.getUsername() + '@' + newComputerName);
		}
	}
}

QWidget* QChatRoomMainWindow::initParticipantsWidget()
{
	QWidget* usersWidget{ new QWidget };
	QSizePolicy modifiedPolicy{ usersWidget->sizePolicy() };
	modifiedPolicy.setHorizontalStretch(1);
	usersWidget->setSizePolicy(modifiedPolicy);
	QVBoxLayout* usersLayout{ new QVBoxLayout };

	QGroupBox* userDisplayNameGroupBox{ new QGroupBox("Your name") };
	QVBoxLayout* userDisplayNameLayout{ new QVBoxLayout };
	userDisplayName = new QLabel(client.getUsername() + '@' + client.getComputerName());
	userDisplayNameLayout->addWidget(userDisplayName);
	userDisplayNameGroupBox->setLayout(userDisplayNameLayout);

	QGroupBox* participantsGroupBox{ new QGroupBox("Participants") };
	QVBoxLayout* participantsLayout{ new QVBoxLayout };
	participantsPanel = new QParticipantsPanel;
	participantsLayout->addWidget(participantsPanel);
	participantsGroupBox->setLayout(participantsLayout);

	usersLayout->addWidget(userDisplayNameGroupBox);
	usersLayout->addWidget(participantsGroupBox);
	usersWidget->setLayout(usersLayout);
	return usersWidget;
}

QChatRoomMainWindow::~QChatRoomMainWindow()
{
	delete serverConnection;
}