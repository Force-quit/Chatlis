#pragma once

#include <QTextEdit>
#include <QString>
#include <QColor>

class QChatbox : public QTextEdit
{
	Q_OBJECT

public:
	QChatbox(QWidget *parent = nullptr);
	~QChatbox();

public slots:
	void appendMessage(const QString& from, const QString& message);
	void userJoined(const QString& nick);
	void userLeft(const QString& nick);

private:
	static const QColor USER_MIGRATION_TEXT_COLOR;
	static const QColor USER_MESSAGE_TEXT_COLOR;

	void appendText(const QString& toAppend);
};