#pragma once

#include <QMainWindow>
#include "QChatlisServer.h"
#include <QGroupBox>
#include <QTextEdit>

class QServerMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	QServerMainWindow();

private:
	QGroupBox* initOutputGroupBox();

	QTextEdit* output;
	QChatlisServer server;
};
