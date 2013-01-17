#pragma once
#include <QtCore/QString>
#include <QtCore/QSet>

class Sequence {
public:
	Sequence() : chars(' '), n(1) {}
	virtual ~Sequence() {}

	void restart(const QString & chars, int n = 1);
	const QChar currentChar() const;
	void markCurrentAsCoincidence();
	bool toNext();
	bool isFinished() const;

	int getN() const { return n; }
	const QString & getChars() const { return chars; }
	const QSet<int> & getCoincidences() const;
	const QSet<int> & getErrors() const;
	const QSet<int> & getMarks() const;
public:
	QString chars;
	int n, current;
	QSet<int> marks, coincidences, errors;
};

