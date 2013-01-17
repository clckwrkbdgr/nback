#include <QtDebug>
#include "settings.h"
#include "mainwindow.h"
	
QString getRandomString(int size, int coincidenceCount, int N)
{
	QString result;
	for(int i = 0; i < size; ++i) {
		result += QChar('A' + qrand() % 26);
	}

	QSet<int> placed;
	for(int i = 0; i < coincidenceCount; ++i) {
		int pos = qrand() % (size - N) + N;
		while(placed.contains(pos)) {
			pos = qrand() % (size - N) + N;
		}

		QChar ch = result[pos - N];
		result[pos] = ch;

		placed << pos;
	}
	return result;
}

QString toString(const QSet<int> & set, int size)
{
	QString result = QString(size, ' ');
	foreach(int value, set) {
		result[value] = '1';
	}
	return result;
}

MainWindow::MainWindow(QWidget * parent)
	: QMainWindow(parent)
{
	Settings::load();
	ui.setupUi(this);
	prepare();
}

MainWindow::~MainWindow()
{
	stop();
}

void MainWindow::on_settings_clicked()
{
	timer.stop();
	Settings dialog;
	dialog.exec();
	stop();
}

void MainWindow::prepare()
{
	ui.run->disconnect(SIGNAL(clicked()));
	ui.stop->disconnect(SIGNAL(clicked()));
	ui.mark->disconnect(SIGNAL(clicked()));

	connect(ui.run, SIGNAL(clicked()), this, SLOT(run()));
	ui.view->setText("");
	sequence.restart(getRandomString(Settings::getSequenceSize(), Settings::getSequenceSize() /4, Settings::getN()), Settings::getN());

	ui.run->setEnabled(true);
	ui.stop->setEnabled(false);
	ui.mark->setEnabled(false);
}

void MainWindow::singleShot(int ms, const char * slot)
{
	timer.stop();
	timer.disconnect(SIGNAL(timeout()));
	connect(&timer, SIGNAL(timeout()), this, slot);
	timer.setSingleShot(true);
	timer.start(ms);
}

void MainWindow::run()
{
	ui.run->disconnect(SIGNAL(clicked()));
	ui.stop->disconnect(SIGNAL(clicked()));
	ui.mark->disconnect(SIGNAL(clicked()));

	ui.view->setText("");
	connect(ui.stop, SIGNAL(clicked()), this, SLOT(stop()));
	connect(ui.mark, SIGNAL(clicked()), this, SLOT(mark()));
	ui.view->setText(sequence.currentChar());
	singleShot(1000, SLOT(clearLetter()));

	ui.run->setEnabled(false);
	ui.stop->setEnabled(true);
	ui.mark->setEnabled(true);
}

void MainWindow::stop()
{
	timer.stop();
	QStringList lines;
	lines << tr("N: %1").arg(Settings::getN());
	lines << tr("Size: %1").arg(Settings::getSequenceSize());
	lines << "";
	if(sequence.isFinished()) {
		lines << tr("Real coincidences: %1, marks set: %2").arg(sequence.getCoincidences().size()).arg(sequence.getMarks().size());
		lines << tr("Wrong marks: %1, missed coincidences: %2") .arg(sequence.getErrors().size()) .arg(sequence.getMissed().size());
	} else {
		lines << tr("Interrupted");
	}
	ui.summary->setPlainText(lines.join("\n"));
	prepare();
}

void MainWindow::clearLetter()
{
	ui.view->setText("");
	singleShot(200, SLOT(next()));
}

void MainWindow::next()
{
	bool isFinished = !sequence.toNext();
	if(isFinished) {
		stop();
	} else {
		ui.view->setText(sequence.currentChar());
		singleShot(1000, SLOT(clearLetter()));
	}
}

void MainWindow::mark()
{
	sequence.markCurrentAsCoincidence();
}

