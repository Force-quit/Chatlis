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
	connect(&mServer, &QChatlisServer::serverLog, mOutputTextEdit, &QTextEdit::append);
}

QGroupBox* QServerMainWindow::initOutputGroupBox()
{
	auto* groupBox{ new QGroupBox("Output") };

	auto* layout{ new QVBoxLayout };
	groupBox->setLayout(layout);

	mOutputTextEdit = new QTextEdit;
	mOutputTextEdit->setReadOnly(true);
	mOutputTextEdit->setLineWrapMode(QTextEdit::NoWrap);
	layout->addWidget(mOutputTextEdit);

	displayIpAddresses();

	return groupBox;
}

void QServerMainWindow::displayIpAddresses()
{
	QString IPV4Addresses;
	bool isFirstItem{ true };
	std::ranges::for_each(QNetworkInterface::allAddresses(), [&IPV4Addresses, &isFirstItem](const QHostAddress& address)
	{
		if (address.protocol() == QHostAddress::NetworkLayerProtocol::IPv4Protocol)
		{
			if (isFirstItem)
			{
				IPV4Addresses += QString("%1").arg(address.toString());
			}
			else
			{
				IPV4Addresses += QString(" | %1").arg(address.toString());
			}

			isFirstItem = false;
		}
	});

	if (!IPV4Addresses.isEmpty())
	{
		mOutputTextEdit->append("Your local ip address(es) : " + IPV4Addresses);
		mOutputTextEdit->append("Log : server listening on port " + QString::number(QChatlisServer::PORT_NB));
	}
	else
	{
		mOutputTextEdit->append("No local ip address found.");
	}
}
