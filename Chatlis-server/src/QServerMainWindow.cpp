#include "QServerMainWindow.hpp"
#include <QVBoxLayout>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTextEdit>
#include <QGroupBox>
#include <algorithm>

QServerMainWindow::QServerMainWindow()
{
	auto* centralWidget{ new QWidget };
	setCentralWidget(centralWidget);

	auto* centralLayout{ new QVBoxLayout };
	centralWidget->setLayout(centralLayout);
	
	QGroupBox* serverLog{ initOutputGroupBox() };
	centralLayout->addWidget(serverLog);

	setWindowTitle("Chatlis - Server");
	setWindowIcon(QIcon(":/images/server.png"));
	resize(600, 350);

	mServer.start();
}

QGroupBox* QServerMainWindow::initOutputGroupBox()
{
	auto* groupBox{ new QGroupBox("Output") };

	auto* layout{ new QVBoxLayout };
	groupBox->setLayout(layout);

	mOutputTextEdit = new QTextEdit;
	layout->addWidget(mOutputTextEdit);
	mOutputTextEdit->setReadOnly(true);
	mOutputTextEdit->setLineWrapMode(QTextEdit::NoWrap);
	connect(&mServer, &QChatlisServer::serverLog, mOutputTextEdit, &QTextEdit::append);

	return groupBox;
}