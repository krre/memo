#pragma once
#include "lib/Exception.h"
#include <QSqlError>
#include <QSqlQuery>

class SqlDatabaseError : MemoLib::Exception {

public:
    SqlDatabaseError(const QSqlError& error);
    QString error() const override;

private:
    QSqlError m_error;
};

class SqlQueryError : MemoLib::Exception {

public:
    SqlQueryError(const QSqlQuery& query);
    QString error() const override;

private:
    QSqlQuery query;
};
