#include <QApplication>
#include <QStyleFactory>
#include "../Headers/QChatRoomMainWindow.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    QApplication::setStyle(QStyleFactory::create("Fusion"));

    QChatRoomMainWindow chatWindow;
    chatWindow.show();
    return app.exec();
}