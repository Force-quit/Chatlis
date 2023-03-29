#pragma once

#include <QMenuBar>

class QChatlisMenuBar : public QMenuBar
{
	Q_OBJECT

public:
	QChatlisMenuBar(QWidget *parent = nullptr);
	~QChatlisMenuBar();

signals:
	void actionConnectToServer();

private:
	
};
