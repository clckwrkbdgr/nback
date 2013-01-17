#include <QtSql/QSqlDatabase>
#include <QtCore/QVariant>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtCore/QTextStream>
#include "statistics.h"

void showError(const QSqlError & error)
{
	QTextStream err(stderr);
	err << error.driverText() << endl;
	err << error.databaseText() << endl;
}

Statistics::Statistics()
	: sessionId(0)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "stat");
	bool ok = db.open();
	if(!ok) {
		showError(db.lastError());
		return;
	}
	QSqlQuery query(db);
	ok = query.exec("CREATE TABLE IF NOT EXISTS nbackSessions (id INT PRIMARY KEY AUTOINCREMENT, startDate TEXT, stopDate TEXT DEFAULT '');");
	if(!ok) showError(query.lastError());
	ok = query.exec("CREATE TABLE IF NOT EXISTS nback (id INT PRIMARY KEY AUTOINCREMENT, sessionId INT, n INT, size INT, errors INT, missed INT);");
	if(!ok) showError(query.lastError());
	ok = query.exec("INSERT INTO nbackSessions (startDate) VALUES (datetime('now'));");
	if(!ok) showError(query.lastError());
	sessionId = query.lastInsertId().toInt();
}

Statistics::~Statistics()
{
	{
		QSqlDatabase db = QSqlDatabase::database("stat");
		QSqlQuery query(db);
		query.prepare("UPDATE nbackSessions SET stopDate = datetime('now')) WHERE id = :id;");
		query.bindValue(":id", sessionId);
		bool ok = query.exec();
		if(!ok) {
			showError(query.lastError());
		}
	}
	QSqlDatabase::removeDatabase("stat");
}

void Statistics::add(int n, int size, int errors, int missed)
{
	QSqlDatabase db = QSqlDatabase::database("stat");
	QSqlQuery query(db);
	query.prepare("INSERT INTO nback (sessionId, n, size, errors, missed) VALUES (:sessionId, :n, :size, :erros, :missed);");
	query.bindValue(":sessionId", sessionId);
	query.bindValue(":n", n);
	query.bindValue(":size", size);
	query.bindValue(":errors", errors);
	query.bindValue(":missed", missed);
	bool ok = query.exec();
	if(!ok) {
		showError(query.lastError());
	}
}

int Statistics::currentTryCount() const
{
	QSqlDatabase db = QSqlDatabase::database("stat");
	QSqlQuery query(db);
	query.prepare("SELECT COUNT(sessionId) FROM nback WHERE sessionId = :sessionId;");
	query.bindValue(":sessionId", sessionId);
	bool ok = query.exec();
	if(!ok) {
		showError(query.lastError());
		return 0;
	}
	query.next();
	return query.value(0).toInt();
}

double Statistics::currentErrorRatio() const
{
	QSqlDatabase db = QSqlDatabase::database("stat");
	QSqlQuery query(db);
	query.prepare("SELECT SUM(errors + missed) / SUM(size) FROM nback WHERE sessionId = :sessionId;");
	query.bindValue(":sessionId", sessionId);
	bool ok = query.exec();
	if(!ok) {
		showError(query.lastError());
		return 0;
	}
	query.next();
	return query.value(0).toDouble();
}

