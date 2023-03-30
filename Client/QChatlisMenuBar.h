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
	void actionDisconnectFromServer();
	void actionChangeUsername();
	void actionChangeComputerName();

private:
	QMenu* initUserSettings();
	QMenu* initServerSettings();
};
