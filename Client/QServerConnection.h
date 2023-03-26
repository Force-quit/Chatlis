#pragma once

#include <QObject>

class QServerConnection  : public QObject
{
	Q_OBJECT

public:
	QServerConnection(QObject *parent);
	~QServerConnection();
};
