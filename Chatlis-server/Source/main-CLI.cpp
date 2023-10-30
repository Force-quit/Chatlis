#include "../Headers/QChatlisServer-CLI.h"
#include <QCoreApplication>
#include <QNetworkInterface>
#include <QLockFile>
#include <QTextStream>
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

	QTextStream cout(stdout);
	if (foundIPV4Addresses.size() > 0)
	{
		foundIPV4Addresses.remove(foundIPV4Addresses.size() - 1, 1);
		cout << "Your local ip address(es) : " << foundIPV4Addresses << '\n';
		cout << "Log : server listening on port " << QString::number(QChatlisServer::PORT_NB) << '\n';
	}
	else
		cout << "No local ip address found.";
	cout.flush();
	return app.exec();
}