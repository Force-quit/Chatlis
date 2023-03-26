// Copyright (C) 2016 The Qt Company Ltd.
// SPDX-License-Identifier: LicenseRef-Qt-Commercial OR BSD-3-Clause

#include <QtWidgets>

#include "chatdialog.h"

ChatDialog::ChatDialog(QWidget* parent)
    : QDialog(parent)
{
    if (objectName().isEmpty())
        setObjectName("ChatDialog");
    resize(513, 349);
    vboxLayout = new QVBoxLayout();

    vboxLayout->setObjectName("vboxLayout");
    hboxLayout = new QHBoxLayout();

    hboxLayout->setObjectName("hboxLayout");
    textEdit = new QTextEdit();
    textEdit->setObjectName("textEdit");
    textEdit->setFocusPolicy(Qt::NoFocus);
    textEdit->setReadOnly(true);

    hboxLayout->addWidget(textEdit);

    listWidget = new QListWidget();
    listWidget->setObjectName("listWidget");
    listWidget->setMaximumSize(QSize(180, 16777215));
    listWidget->setFocusPolicy(Qt::NoFocus);

    hboxLayout->addWidget(listWidget);


    vboxLayout->addLayout(hboxLayout);

    hboxLayout1 = new QHBoxLayout();
    hboxLayout1->setContentsMargins(0, 0, 0, 0);
    hboxLayout1->setObjectName("hboxLayout1");
    label = new QLabel();
    label->setObjectName("label");

    hboxLayout1->addWidget(label);

    lineEdit = new QLineEdit();
    lineEdit->setObjectName("lineEdit");

    hboxLayout1->addWidget(lineEdit);


    vboxLayout->addLayout(hboxLayout1);


    this->setWindowTitle(QCoreApplication::translate("ChatDialog", "Chat", nullptr));
    label->setText(QCoreApplication::translate("ChatDialog", "Message:", nullptr));

    QMetaObject::connectSlotsByName(this);

    lineEdit->setFocusPolicy(Qt::StrongFocus);
    textEdit->setFocusPolicy(Qt::NoFocus);
    textEdit->setReadOnly(true);
    listWidget->setFocusPolicy(Qt::NoFocus);

    connect(lineEdit, &QLineEdit::returnPressed,
        this, &ChatDialog::returnPressed);
    connect(&client, &Client::newMessage,
        this, &ChatDialog::appendMessage);
    connect(&client, &Client::newParticipant,
        this, &ChatDialog::newParticipant);
    connect(&client, &Client::participantLeft,
        this, &ChatDialog::participantLeft);

    myNickName = client.nickName();
    newParticipant(myNickName);
    tableFormat.setBorder(0);
    QTimer::singleShot(10 * 1000, this, SLOT(showInformation()));
}

void ChatDialog::appendMessage(const QString& from, const QString& message)
{
    if (from.isEmpty() || message.isEmpty())
        return;

    QTextCursor cursor(textEdit->textCursor());
    cursor.movePosition(QTextCursor::End);
    QTextTable* table = cursor.insertTable(1, 2, tableFormat);
    table->cellAt(0, 0).firstCursorPosition().insertText('<' + from + "> ");
    table->cellAt(0, 1).firstCursorPosition().insertText(message);
    QScrollBar* bar = textEdit->verticalScrollBar();
    bar->setValue(bar->maximum());
}

void ChatDialog::returnPressed()
{
    QString text = lineEdit->text();
    if (text.isEmpty())
        return;

    if (text.startsWith(QChar('/'))) 
    {
        QColor color = textEdit->textColor();
        textEdit->setTextColor(Qt::red);
        textEdit->append(tr("! Unknown command: %1").arg(text.left(text.indexOf(' '))));
        textEdit->setTextColor(color);
    }
    else 
    {
        client.sendMessage(text);
        appendMessage(myNickName, text);
    }

    lineEdit->clear();
}

void ChatDialog::newParticipant(const QString& nick)
{
    if (nick.isEmpty())
        return;

    QColor color = textEdit->textColor();
    textEdit->setTextColor(Qt::gray);
    textEdit->append(tr("* %1 has joined").arg(nick));
    textEdit->setTextColor(color);
    listWidget->addItem(nick);
}

void ChatDialog::participantLeft(const QString& nick)
{
    if (nick.isEmpty())
        return;

    QList<QListWidgetItem*> items = listWidget->findItems(nick,
        Qt::MatchExactly);
    if (items.isEmpty())
        return;

    delete items.at(0);
    QColor color = textEdit->textColor();
    textEdit->setTextColor(Qt::gray);
    textEdit->append(tr("* %1 has left").arg(nick));
    textEdit->setTextColor(color);
}

void ChatDialog::showInformation()
{
    if (listWidget->count() == 1) {
        QMessageBox::information(this, tr("Chat"),
            tr("Launch several instances of this "
                "program on your local network and "
                "start chatting!"));
    }
}