#include "QChatRoomMainWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QSplitter>
#include "QChatbox.h"
#include <QTimer>
#include <QIcon>
#include "QParticipantsPanel.h"
#include <QLineEdit>

QChatRoomMainWindow::QChatRoomMainWindow(QWidget *parent)
	: QMainWindow(parent), chatbox{ new QChatbox }
{
	QWidget* centralWidget{ new QWidget };
	QVBoxLayout* centralLayout{ new QVBoxLayout };

	QSplitter* topLayout{ new QSplitter };
	topLayout->setChildrenCollapsible(false);
	QParticipantsPanel* participantsPanel{ new QParticipantsPanel };
	topLayout->addWidget(chatbox);
	topLayout->addWidget(participantsPanel);

	QHBoxLayout* textInputLayout{ new QHBoxLayout };
	QLabel* textInputLabel{ new QLabel(tr("Message :")) };
	QLineEdit* textInput{ new QLineEdit };
	textInputLayout->addWidget(textInputLabel);
	textInputLayout->addWidget(textInput);
	
	centralLayout->addWidget(topLayout);
	centralLayout->addLayout(textInputLayout);
	centralWidget->setLayout(centralLayout);
	setCentralWidget(centralWidget);

	connect(textInput, &QLineEdit::returnPressed, [=]() {chatbox->appendMessage("Emalice", textInput->text()); });

	connect(textInput, &QLineEdit::returnPressed, textInput, &QLineEdit::clear);


	resize(650, 350);
	setCentralWidget(centralWidget);
	setWindowTitle("Chatlis");
	setWindowIcon(QIcon("group-chat.png"));
}

QChatRoomMainWindow::~QChatRoomMainWindow() {}