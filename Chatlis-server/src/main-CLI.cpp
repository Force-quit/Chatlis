#include <QCoreApplication>
#include "QChatlisServer.hpp"
#include <QNetworkInterface>
#include <QLockFile>
#include <QTextStream>
#include <QDir>

int main(int argc, char* argv[])
{
    QLockFile lockFile(QDir::tempPath() + "/ChatlisServer.lock");
	if (!lockFile.tryLock())
	{
        return 0;
	}

	QCoreApplication app(argc, argv);
	QChatlisServer server;
	QTextStream cout(stdout);
	QObject::connect(&server, &QChatlisServer::serverLog, [&cout](const QString& message)
	{
		cout << message << '\n';
		cout.flush();
	});

	server.start();
	return app.exec();
}