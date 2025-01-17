#pragma once

#include "QClientInfo.h"
#include <QTextEdit>
#include <QString>
#include <QColor>


class QChatbox : public QTextEdit
{
	Q_OBJECT

public:
	QChatbox(const QClientInfo& clientInfo, QWidget* parent = nullptr);
	~QChatbox();

public slots:
	void appendUserMessage(const QString& from, const QString& message);
	void appendSystemMessage(const QString message);
	void appendServerMessage(const QString message);

private:
	static const QColor USER_MESSAGE_TEXT_COLOR;
	static const QColor SERVER_MESSAGE_TEXT_COLOR;
	static const QColor SYSTEM_MESSAGE_TEXT_COLOR;
	static const QColor USER_MESSAGE_PING_BG_COLOR;
	
	const QClientInfo& clientInfo;

	void appendTextWithTime(const QString& toAppend);
};