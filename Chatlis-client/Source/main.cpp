#include <QApplication>
#include "../Headers/QChatRoomMainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    QChatRoomMainWindow chatWindow;
    chatWindow.show();
    return app.exec();
}