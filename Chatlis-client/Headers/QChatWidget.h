#pragma once

#include <QWidget>
#include "QChatbox.h"
#include <QLineEdit>
#include "../../Common/QClientInfo.h"

class QChatWidget : public QWidget
{
	Q_OBJECT

public:
	QChatWidget(QWidget* parent, const QClientInfo& clientInfo);
	~QChatWidget();

	void clearMessages();

signals:
	void appendUserMessage(const QString& from, const QString& message);
	void appendSystemMessage(const QString message);
	void appendServerMessage(const QString message);
	void sendMessage(const QString& message);

private slots:
	void handleReturnPressed();

private:
	QChatbox* chatBox;
	QLineEdit* textInput;
	const QClientInfo& clientInfo;
};
