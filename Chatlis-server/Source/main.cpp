#include <QApplication>
#include "../Headers/QServerMainWindow.h"
#include <QLockFile>
#include <QDir>

int main(int argc, char* argv[])
{
    QLockFile lockFile(QDir::tempPath() + "/ChatlisServer.lock");
    if (!lockFile.tryLock()) 
        return 0;

    QApplication app(argc, argv);

    QServerMainWindow serverWindow;
    serverWindow.show();
    return app.exec();
}