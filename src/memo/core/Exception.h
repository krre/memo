#pragma once
#include "lib/Exception.h"
#include <QSqlError>
#include <QSqlQuery>

class SqlDatabaseError : MemoLib::Exception {

public:
    SqlDatabaseError(const QSqlError& error);
    QString text() const override;

private:
    QSqlError error;
};

class SqlQueryError : MemoLib::Exception {

public:
    SqlQueryError(const QSqlQuery& query);
    QString text() const override;

private:
    QSqlQuery query;
};
