#include "QChatlisMenuBar.h"
#include <QAction>
#include <QKeySequence>

QChatlisMenuBar::QChatlisMenuBar(QWidget *parent)
	: QMenuBar(parent)
{
	QMenu* userSettings{ initUserSettings() };
	addMenu(userSettings);

	auto* connectAction{ new QAction("Connect to server", this) };
	connectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_C));
	connect(connectAction, SIGNAL(triggered()), this, SIGNAL(actionConnectToServer()));
	addAction(connectAction);
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

QChatlisMenuBar::~QChatlisMenuBar()
{

}