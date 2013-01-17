#pragma once
#include <QtGui/QDialog>
#include "ui_settings.h"

class Settings : public QDialog {
	Q_OBJECT
	Q_DISABLE_COPY(Settings);
public:
	Settings(QWidget * parent = 0);
	virtual ~Settings();

	static void load();

	static int getN() { return n; }
	static int getSequenceSize() { return sequenceSize; }
private:
	Ui::Settings ui;
	static int n;
	static int sequenceSize;
};
