#include "QChatbox.h"
#include <QTextCursor>
#include <QTextTable>
#include <QScrollBar>
#include <QColor>
#include <QDateTime>

const QColor QChatbox::USER_MIGRATION_TEXT_COLOR{ Qt::gray };
const QColor QChatbox::USER_MESSAGE_TEXT_COLOR{ Qt::black };

QChatbox::QChatbox(QWidget *parent)
	: QTextEdit(parent)
{
    setFocusPolicy(Qt::NoFocus);
    setReadOnly(true);
    setTextColor(QChatbox::USER_MESSAGE_TEXT_COLOR);
}

void QChatbox::userJoined(const QString & nick)
{
    setTextColor(QChatbox::USER_MIGRATION_TEXT_COLOR);
    appendText(tr("%1 has joined").arg(nick));
    setTextColor(QChatbox::USER_MESSAGE_TEXT_COLOR);
}

void QChatbox::userLeft(const QString& nick)
{
    setTextColor(QChatbox::USER_MIGRATION_TEXT_COLOR);
    appendText(tr("%1 has left").arg(nick));
    setTextColor(QChatbox::USER_MESSAGE_TEXT_COLOR);
}

void QChatbox::appendMessage(const QString& from, const QString& message)
{
    QScrollBar* bar{ verticalScrollBar() };
    bool scrollBarWasAtBottom{ bar->value() == bar->maximum() };

    QString toAppend{ "[%1] %2" };
    appendText(toAppend.arg(from, message));

    if (scrollBarWasAtBottom)
        bar->setValue(bar->maximum());
}

void QChatbox::appendText(const QString& toAppend)
{
    QString timeStampedMessage{ "%1 %2" };
    append(timeStampedMessage.arg(QDateTime::currentDateTime().toString("hh:mm"), toAppend));
}

QChatbox::~QChatbox() {}