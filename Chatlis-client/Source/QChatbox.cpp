#include "../Headers/QChatbox.h"
#include <QTextCursor>
#include <QTextTable>
#include <QApplication>
#include <QScrollBar>
#include <QColor>
#include <QDateTime>
#include <QSizePolicy>

const QColor QChatbox::SERVER_MESSAGE_TEXT_COLOR{ Qt::gray };
const QColor QChatbox::SYSTEM_MESSAGE_TEXT_COLOR{ Qt::red };
const QColor QChatbox::USER_MESSAGE_PING_BG_COLOR{ QColor(0,0,255,50)};


QChatbox::QChatbox(QWidget *parent, const QClientInfo& clientInfo)
	: QTextEdit(parent), clientInfo(clientInfo)
{
    setFocusPolicy(Qt::NoFocus);
    setReadOnly(true);
}

void QChatbox::appendTextWithTime(const QString& toAppend)
{
    QString timeStampedMessage{ "%1 %2" };
    append(timeStampedMessage.arg(QDateTime::currentDateTime().toString("hh:mm"), toAppend));
}

void QChatbox::appendSystemMessage(const QString message)
{
    setTextColor(QChatbox::SYSTEM_MESSAGE_TEXT_COLOR);
    appendTextWithTime("System : " + message);
}

void QChatbox::appendServerMessage(const QString message)
{
    setTextColor(QChatbox::SERVER_MESSAGE_TEXT_COLOR);
    appendTextWithTime("Server : " + message);
}

void QChatbox::appendUserMessage(const QString& from, const QString& message)
{
    setTextColor(QApplication::palette().text().color());
    if (message.contains(QString("@" + clientInfo.getUsername()))) {
        setTextBackgroundColor(QChatbox::USER_MESSAGE_PING_BG_COLOR);
    }

    QScrollBar* bar{ verticalScrollBar() };
    bool scrollBarWasAtBottom{ bar->value() == bar->maximum() };

    QString toAppend{ "[%1] %2" };
    appendTextWithTime(toAppend.arg(from, message));
    setTextBackgroundColor(Qt::transparent);

    if (scrollBarWasAtBottom)
        bar->setValue(bar->maximum());
}

QChatbox::~QChatbox() {}