#include "Database.h"
#include <QtSql>

Database::Database(QObject* parent) : QObject(parent) {
    db = QSqlDatabase::addDatabase("QSQLITE");
}

Database::~Database() {
    close();
}

void Database::create(const QString& filepath) {
    qDebug() << "create database" << filepath;
    db.close();
    db.setDatabaseName(filepath);
    if (!db.open()) {
        qDebug() << db.lastError();
        return;
    }

    QSqlQuery query = db.exec("CREATE TABLE notes(id INTEGER)");
    if (query.lastError().type() != QSqlError::NoError) {
        qDebug() << query.lastError().text();
    }
}

void Database::open(const QString& filepath) {
    qDebug() << "open database" << filepath;
    db.close();
    db.setDatabaseName(filepath);
    if (!db.open()) {
        qDebug() << db.lastError();
    }
}

void Database::close() {
    qDebug() << "close database";
    db.close();
}
