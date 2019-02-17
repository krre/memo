#include "Database.h"
#include <QtSql>

Database::Database(QObject* parent) : QObject(parent) {
    db = QSqlDatabase::addDatabase("QSQLITE");
}

Database::~Database() {
    close();
}

bool Database::create(const QString& filepath) {
    qInfo().noquote() << "Create database:" << filepath;
    db.close();
    db.setDatabaseName(filepath);
    if (!db.open()) {
        databaseError(db.lastError());
        return false;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE notes("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "parent INTEGER,"
                    "pos INTEGER,"
                    "title TEXT,"
                    "note TEXT,"
                    "created_at TIMESTAMP,"
                    "updated_at TIMESTAMP"
                    ")")) {
        queryError(query);
        return false;
    }

    return true;
}

bool Database::open(const QString& filepath) {
    qDebug().noquote() << "Open database:" << filepath;
    db.close();
    db.setDatabaseName(filepath);
    if (!db.open()) {
        databaseError(db.lastError());
        return false;
    }

    return true;
}

void Database::close() {
    if (db.isOpen()) {
        qInfo().noquote() << "Close database:" << db.databaseName();
        db.close();
    }
}

void Database::databaseError(const QSqlError& error) {
    qCritical("Database error: %s. %s", qUtf8Printable(error.databaseText()), qUtf8Printable(error.driverText()));
}

void Database::queryError(const QSqlQuery& query) {
    qCritical("SQL query error: %s. %s. %s", qUtf8Printable(query.lastError().databaseText()), qUtf8Printable(query.lastError().driverText()), qUtf8Printable(query.lastQuery()));
}
