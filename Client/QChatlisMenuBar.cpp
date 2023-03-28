#include "QChatlisMenuBar.h"
#include <QAction>
#include <QKeySequence>

QChatlisMenuBar::QChatlisMenuBar(QWidget *parent)
	: QMenuBar(parent)
{
	auto* connectAction{ new QAction("Connect to server", this) };
	connectAction->setShortcut(QKeySequence(Qt::CTRL | Qt::ALT | Qt::Key_C));
	connect(connectAction, SIGNAL(triggered()), this, SIGNAL(actionConnectToServer()));
	addAction(connectAction);
}

QChatlisMenuBar::~QChatlisMenuBar()
{}