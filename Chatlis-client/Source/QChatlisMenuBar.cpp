#include "../Headers/QChatlisMenuBar.h"
#include <QAction>
#include <QKeySequence>

QChatlisMenuBar::QChatlisMenuBar(QWidget *parent)
	: QMenuBar(parent)
{
	QMenu* userSettings{ initUserSettings() };
	addMenu(userSettings);

	QMenu* serverSettings{ initServerSettings() };
	addMenu(serverSettings);
}

QMenu* QChatlisMenuBar::initUserSettings()
{
	QMenu* userSettings{ new QMenu("User settings", this) };
	
	QAction* changeUsernameAction{ new QAction("Displayed username", userSettings) };
	connect(changeUsernameAction, SIGNAL(triggered()), this, SIGNAL(actionChangeUsername()));
	userSettings->addAction(changeUsernameAction);

	QAction* changeComputerNameAction{ new QAction("Displayed computer name", userSettings) };
	connect(changeComputerNameAction, SIGNAL(triggered()), this, SIGNAL(actionChangeComputerName()));
	userSettings->addAction(changeComputerNameAction);

	return userSettings;
}

QMenu* QChatlisMenuBar::initServerSettings()
{
	QMenu* serverSettings{ new QMenu("Server settings", this) };

	auto* connectAction{ new QAction("Connect to server", this) };
	connectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_C));
	connect(connectAction, SIGNAL(triggered()), this, SIGNAL(actionConnectToServer()));
	serverSettings->addAction(connectAction);

	auto* disconnectAction{ new QAction("Disconnect from server", this) };
	disconnectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_D));
	connect(disconnectAction, SIGNAL(triggered()), this, SIGNAL(actionDisconnectFromServer()));
	serverSettings->addAction(disconnectAction);

	return serverSettings;
}

QChatlisMenuBar::~QChatlisMenuBar()
{

}