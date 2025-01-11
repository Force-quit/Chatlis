#pragma once

#include <QMainWindow>
#include "QChatlisServer.hpp"
#include <QGroupBox>
#include <QTextEdit>

class QServerMainWindow : public QMainWindow
{
	Q_OBJECT

public:
	QServerMainWindow();

private:
	QGroupBox* initOutputGroupBox();

	QTextEdit* mOutputTextEdit{};
	QChatlisServer mServer;
};
