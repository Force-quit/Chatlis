#include "../Headers/QServerMainWindow.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QTextEdit>
#include <QStringList>
#include <QGroupBox>

QServerMainWindow::QServerMainWindow()
	: QMainWindow(), server(), output{}
{
	QGroupBox* serverLog{ initOutputGroupBox() };

	setCentralWidget(serverLog);
	setWindowTitle("Chatlis - Server");
	resize(500, 200);

	connect(&server, &QChatlisServer::serverLog, output, &QTextEdit::append);
}


QGroupBox* QServerMainWindow::initOutputGroupBox()
{
	QGroupBox* outputGroupBox{ new QGroupBox };
	outputGroupBox->setFocusPolicy(Qt::NoFocus);
	QVBoxLayout* outputGroupBoxLayout{ new QVBoxLayout };

	output = new QTextEdit;
	output->setReadOnly(true);
	output->setLineWrapMode(QTextEdit::NoWrap);

	outputGroupBoxLayout->addWidget(output);
	outputGroupBox->setLayout(outputGroupBoxLayout);

	QString foundIPV4Addresses;
	for (const QHostAddress& address : QNetworkInterface::allAddresses())
	{
		if (address.protocol() == QHostAddress::NetworkLayerProtocol::IPv4Protocol)
		{
			foundIPV4Addresses += ' ' + address.toString() + " |";
		}
	}

	if (foundIPV4Addresses.size() > 0)
	{
		foundIPV4Addresses.remove(foundIPV4Addresses.size() - 1, 1);
		output->append("Your local ip address(es) : " + foundIPV4Addresses);
		output->append("Log : server listening on port " + QString::number(QChatlisServer::PORT_NB));
	}
	else
	{
		output->append("No local ip address found.");
	}

	return outputGroupBox;
}