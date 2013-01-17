#include <QtDebug>
#include "sequence.h"

void Sequence::restart(const QString & chars, int n)
{
	this->chars = chars;
	if(this->chars.isEmpty()) {
		this->chars = ' ';
	}
	this->n = n > 0 ? n : 1;
	current = 0;

	marks.clear();
	coincidences.clear();
	errors.clear();

	for(int i = n; i < chars.count(); ++i) {
		if(chars[i] == chars[i - n]) {
			coincidences << i;
		}
	}
}

const QChar Sequence::currentChar() const
{
	return chars.at(current);
}

void Sequence::markCurrentAsCoincidence()
{
	marks << current;
	if(!coincidences.contains(current)) {
		errors << current;
	}
}

bool Sequence::isFinished() const
{
	return (current >= chars.count());
}

bool Sequence::toNext()
{
	if(current >= chars.count())
		return false;

	if(coincidences.contains(current) && !marks.contains(current)) {
		errors << current;
	}

	++current;
	if(current >= chars.count())
		return false;

	return true;
}

const QSet<int> & Sequence::getCoincidences() const
{
	return coincidences;
}

const QSet<int> & Sequence::getErrors() const
{
	return errors;
}

const QSet<int> & Sequence::getMarks() const
{
	return marks;
}


// Use qmake "CONFIG+=SEQUENCE_TEST"
#ifdef SEQUENCE_TEST
#include <QtTest/QtTest>

class TestSequence : public QObject {
	Q_OBJECT
private slots:
	void init() {
		finished = false;
	}
	void testCurrent_data() {
		QString chars = "ASDFARGAGASGASFG";
		sequence.restart(chars);

		QTest::addColumn<QChar>("ch");
		foreach(const QChar & ch, chars) {
			QTest::newRow(QString(ch).toLatin1().data()) << ch;
		}
	}
	void testCurrent() {
		QFETCH(QChar, ch);
		QCOMPARE(sequence.currentChar(), ch);
		QVERIFY(!finished);
		sequence.markCurrentAsCoincidence();
		finished = !sequence.toNext();
	}
	void testResult_data() {
		QTest::addColumn<QString>("chars");
		QTest::addColumn<int>    ("n");
		QTest::addColumn<QString>("markString");
		QTest::addColumn<QString>("expectedErrors");
		QTest::addColumn<QString>("expectedCoincidences");

		QTest::newRow("1: no errors")
			                 << QString("ASDFAADFFGDGDDG")
			                 << 1
			                 << QString("     1  1    1 ")
			                 << QString("               ")
			                 << QString("     1  1    1 ");
		QTest::newRow("1: errors")
			                 << QString("ASDFAADFFGDGDDG")
			                 << 1
			                 << QString("  1   1 1   11 ")
			                 << QString("  1  11     1  ")
			                 << QString("     1  1    1 ");
		QTest::newRow("2: errors")
			                 << QString("ASDFAADFFGDGDDG")
			                 << 2
			                 << QString("     1  1  1 11")
			                 << QString("     1  1   111")
			                 << QString("           11  ");
		QTest::newRow("4: no errors")
			                 << QString("ASDFAADFFGDGDDG")
			                 << 4
			                 << QString("    1  1       ")
			                 << QString("      1   1    ")
			                 << QString("    1 11  1    ");
	}
	void testResult() {
		QFETCH(QString, chars);
		QFETCH(int,     n);
		QFETCH(QString, markString);
		QFETCH(QString, expectedErrors);
		QFETCH(QString, expectedCoincidences);

		QList<bool> marks = toBoolList(markString);
		QCOMPARE(chars.count(), marks.count());

		sequence.restart(chars, n);
		for(int i = 0; i < chars.count(); ++i) {
			if(marks[i]) {
				sequence.markCurrentAsCoincidence();
			}
			QVERIFY(!finished);
			finished = !sequence.toNext();
		}
		QVERIFY(finished);

		QString result       = toString(sequence.getMarks(),        chars.count()); QCOMPARE(result,       markString);
		QString errors       = toString(sequence.getErrors(),       chars.count()); QCOMPARE(errors,       expectedErrors);
		QString coincidences = toString(sequence.getCoincidences(), chars.count()); QCOMPARE(coincidences, expectedCoincidences);
	}
private:
	Sequence sequence;
	bool finished;

	QList<bool> toBoolList(const QString & string) {
		QList<bool> result;
		foreach(const QChar & ch, string) {
			result << (ch != ' ');
		}
		return result;
	}
	QString toString(const QSet<int> & set, int size) {
		QString result = QString(size, ' ');
		foreach(int value, set) {
			result[value] = '1';
		}
		return result;
	}
};

QTEST_MAIN(TestSequence)
#include "sequence.moc"
#endif//SEQUENCE_TEST
