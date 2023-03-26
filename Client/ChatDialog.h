#pragma once

#include "Client.h"
#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>

class ChatDialog : public QDialog
{
    Q_OBJECT

public:
    ChatDialog(QWidget* parent = nullptr);

public slots:
    void appendMessage(const QString& from, const QString& message);

private slots:
    void returnPressed();
    void newParticipant(const QString& nick);
    void participantLeft(const QString& nick);
    void showInformation();

private:
    Client client;
    QString myNickName;
    QTextTableFormat tableFormat;


    QVBoxLayout* vboxLayout;
    QHBoxLayout* hboxLayout;
    QTextEdit* textEdit;
    QListWidget* listWidget;
    QHBoxLayout* hboxLayout1;
    QLabel* label;
    QLineEdit* lineEdit;
};