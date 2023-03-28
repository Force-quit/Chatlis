#include "QChatbox.h"
#include <QTextCursor>
#include <QTextTable>
#include <QScrollBar>
#include <QColor>
#include <QDateTime>
#include <QSizePolicy>

const QColor QChatbox::SERVER_MESSAGE_TEXT_COLOR{ Qt::gray };
const QColor QChatbox::USER_MESSAGE_TEXT_COLOR{ Qt::black };
const QColor QChatbox::SYSTEM_MESSAGE_TEXT_COLOR{ Qt::red };


QChatbox::QChatbox(QWidget *parent)
	: QTextEdit(parent)
{
    QSizePolicy modifiedPolicy{ sizePolicy() };
    modifiedPolicy.setHorizontalStretch(4);
    setSizePolicy(modifiedPolicy);

    setFocusPolicy(Qt::NoFocus);
    setReadOnly(true);
    setTextColor(QChatbox::USER_MESSAGE_TEXT_COLOR);
}

void QChatbox::appendUserMessage(const QString& from, const QString& message)
{
    QScrollBar* bar{ verticalScrollBar() };
    bool scrollBarWasAtBottom{ bar->value() == bar->maximum() };

    QString toAppend{ "[%1] %2" };
    appendTextWithTime(toAppend.arg(from, message));

    if (scrollBarWasAtBottom)
        bar->setValue(bar->maximum());
}

void QChatbox::appendTextWithTime(const QString& toAppend)
{
    QString timeStampedMessage{ "%1 %2" };
    append(timeStampedMessage.arg(QDateTime::currentDateTime().toString("hh:mm"), toAppend));
}

void QChatbox::clearChat()
{
    clear();
}

void QChatbox::appendSystemMessage(const QString message)
{
    setTextColor(QChatbox::SYSTEM_MESSAGE_TEXT_COLOR);
    appendTextWithTime("System : " + message);
    setTextColor(QChatbox::USER_MESSAGE_TEXT_COLOR);
}

void QChatbox::appendServerMessage(const QString message)
{
    setTextColor(QChatbox::SERVER_MESSAGE_TEXT_COLOR);
    appendTextWithTime("Server : " + message);
    setTextColor(QChatbox::USER_MESSAGE_TEXT_COLOR);
}

QChatbox::~QChatbox() {}