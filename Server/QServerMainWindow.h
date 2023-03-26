#pragma once

#include <QMainWindow>
#include "QChatlisServer.h"

class QServerMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	QServerMainWindow(QWidget *parent = nullptr);
	~QServerMainWindow();

private:
	QChatlisServer* server;
};
