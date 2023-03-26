#include "QClientMessage.h"

QClientMessage::QClientMessage(QObject* parent, const QString& user, const QString& messageContent)
	: QObject(parent), USER(user), MESSAGE_CONTENT(messageContent)
{}

QClientMessage::~QClientMessage() {}