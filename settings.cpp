#include <QtCore/QSettings>
#include "settings.h"

int Settings::n = 1;
int Settings::sequenceSize = 10;

Settings::Settings(QWidget * parent)
	: QDialog(parent)
{
	ui.setupUi(this);
	load();
	ui.n->setValue(n);
	ui.sequenceSize->setValue(sequenceSize);
}

Settings::~Settings()
{
	if(result() == QDialog::Accepted) {
		n = ui.n->value();
		sequenceSize = ui.sequenceSize->value();
	}
	QSettings settings;
	settings.setValue("sequence/n", n);
	settings.setValue("sequence/size", sequenceSize);
}

void Settings::load()
{
	QSettings settings;
	n = settings.value("sequence/n", n).toInt();
	sequenceSize = settings.value("sequence/size", sequenceSize).toInt();
}
