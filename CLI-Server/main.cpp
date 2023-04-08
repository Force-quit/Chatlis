#include "QChatlisServer.h"
#include <QCoreApplication>
#include <QNetworkInterface>
#include <iostream>
#include <QLockFile>
#include <QDir>

int main(int argc, char* argv[])
{
    QLockFile lockFile(QDir::tempPath() + "/ChatlisServer.lock");
    if (!lockFile.tryLock()) 
        return 0;

	QCoreApplication app(argc, argv);

	QChatlisServer server;

	QString foundIPV4Addresses;
	for (const QHostAddress& address : QNetworkInterface::allAddresses())
		if (address.protocol() == QHostAddress::NetworkLayerProtocol::IPv4Protocol)
			foundIPV4Addresses += ' ' + address.toString() + " |";

	if (foundIPV4Addresses.size() > 0)
	{
		foundIPV4Addresses.remove(foundIPV4Addresses.size() - 1, 1);
		std::cout << "Your local ip address(es) : " + foundIPV4Addresses.toStdString() << std::endl;
		std::cout << "Log : server listening on port " + QString::number(QChatlisServer::PORT_NB).toStdString() << std::endl;
	}
	else
		std::cout << "No local ip address found.";

	return app.exec();
}