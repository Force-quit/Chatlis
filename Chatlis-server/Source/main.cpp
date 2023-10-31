#include <QApplication>
#include "../Headers/QServerMainWindow.h"
#include <QStyleFactory>
#include <QLockFile>
#include <QDir>

int main(int argc, char* argv[])
{
    QLockFile lockFile(QDir::tempPath() + "/ChatlisServer.lock");
    if (!lockFile.tryLock()) 
        return 0;

    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QServerMainWindow serverWindow;
    serverWindow.show();
    return app.exec();
}