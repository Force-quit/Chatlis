#include <QApplication>
#include "QChatRoomMainWindow.h"
#include <QLockFile>
#include <QDir>

int main(int argc, char* argv[])
{
    QLockFile lockFile(QDir::tempPath() + "/ChatlisClient.lock");
    if (!lockFile.tryLock()) 
        return 0;

    QApplication app(argc, argv);

    QChatRoomMainWindow chatWindow;
    chatWindow.show();
    return app.exec();
}