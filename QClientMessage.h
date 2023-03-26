#pragma once

#include <QObject>
#include <QString>

class QClientMessage  : public QObject
{
	Q_OBJECT

public:
	QClientMessage(QObject *parent, const QString& user, const QString& messageContent);
	~QClientMessage();
	const QString USER;
	const QString MESSAGE_CONTENT;
};
