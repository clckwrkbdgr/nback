#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char ** argv)
{
	qsrand(time(NULL));
	QApplication app(argc, argv);
	app.setOrganizationName("kp580bm1");
	app.setApplicationName("nback");

	MainWindow wnd;
	wnd.show();
	return app.exec();
}
