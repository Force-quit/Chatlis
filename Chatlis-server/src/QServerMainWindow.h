#pragma once

#include <QMainWindow>
#include "QChatlisServer.h"
#include <QGroupBox>
#include <QTextEdit>

class QServerMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	QServerMainWindow(QWidget *parent = nullptr);
	~QServerMainWindow();

private:
	QChatlisServer* server;
	QTextEdit* output;

	QGroupBox* initOutputGroupBox();
};
