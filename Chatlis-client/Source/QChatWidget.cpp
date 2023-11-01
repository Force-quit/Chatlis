#include "../Headers/QChatWidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QGroupBox>
#include <QLineEdit>

QChatWidget::QChatWidget(QWidget *parent, const QClientInfo& clientInfo)
	: QWidget(parent), chatBox{ new QChatbox(this, clientInfo) }, textInput{ new QLineEdit(this) }, clientInfo(clientInfo)
{
	QSizePolicy modifiedPolicy{ sizePolicy() };
	modifiedPolicy.setHorizontalStretch(2);
	setSizePolicy(modifiedPolicy);

	QVBoxLayout* mainLayout{ new QVBoxLayout };
	
	QGroupBox* chatBoxGroupBox{ new QGroupBox("Chat") };
	QVBoxLayout* chatBoxGroupBoxLayout{ new QVBoxLayout };
	chatBoxGroupBoxLayout->addWidget(chatBox);
	chatBoxGroupBox->setLayout(chatBoxGroupBoxLayout);

	QHBoxLayout* textInputLayout{ new QHBoxLayout };
	QPushButton* sendMessageButton{ new QPushButton("Send", this) };
	textInputLayout->addWidget(textInput);
	textInputLayout->addWidget(sendMessageButton);

	mainLayout->addWidget(chatBoxGroupBox);
	mainLayout->addLayout(textInputLayout);
	setLayout(mainLayout);

	connect(this, &QChatWidget::appendSystemMessage, chatBox, &QChatbox::appendSystemMessage);
	connect(this, &QChatWidget::appendUserMessage, chatBox, &QChatbox::appendUserMessage);
	connect(this, &QChatWidget::appendServerMessage, chatBox, &QChatbox::appendServerMessage);

	connect(sendMessageButton, &QPushButton::clicked, textInput, &QLineEdit::returnPressed);
	connect(textInput, &QLineEdit::returnPressed, this, &QChatWidget::handleReturnPressed);
}

void QChatWidget::handleReturnPressed()
{
	QString currentText(textInput->text().simplified());
	if (!currentText.isEmpty())
	{
		appendUserMessage(clientInfo.getUsername(), currentText);
		emit sendMessage(currentText);
	}
	textInput->clear();
}

void QChatWidget::clearMessages()
{
	chatBox->clear();
}

QChatWidget::~QChatWidget() {}