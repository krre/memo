#include "Database.h"
#include <QtCore>

Database::Database(QObject* parent) : QObject(parent) {

}

void Database::create(const QString& filepath) {
    qDebug() << "create database" << filepath;
}

void Database::open(const QString& filepath) {
    qDebug() << "open database" << filepath;
}

void Database::close() {
    qDebug() << "close database";
}
