#include "QServerMainWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTextEdit>
#include <QGroupBox>

QServerMainWindow::QServerMainWindow(QWidget *parent)
	: QMainWindow(parent), server{new QChatlisServer }
{
	QWidget* centralWidget{ new QWidget };
	QVBoxLayout* centralLayout{ new QVBoxLayout };
	centralLayout->setContentsMargins(10, 10, 10, 10);

	QGroupBox* outputGroupBox{ new QGroupBox("Output") };
	outputGroupBox->setFocusPolicy(Qt::NoFocus);
	QVBoxLayout* outputGroupBoxLayout{ new QVBoxLayout };

	QTextEdit* output{ new QTextEdit };
	output->setReadOnly(true);
	output->setLineWrapMode(QTextEdit::NoWrap);

	outputGroupBoxLayout->addWidget(output);
	outputGroupBox->setLayout(outputGroupBoxLayout);


	for (const QHostAddress& address : QNetworkInterface::allAddresses())
		output->append("IP address found : " + address.toString());

	output->append("Log : server listening on port " + QString::number(server->listeningPort()));

	connect(server, &QChatlisServer::newOutput, output, &QTextEdit::append);
	centralLayout->addWidget(outputGroupBox);
	centralWidget->setLayout(centralLayout);
	setCentralWidget(centralWidget);
	setWindowTitle("Chatlis - Server");
	resize(400, 200);
}

QServerMainWindow::~QServerMainWindow() 
{
	delete server;
}