#pragma once
#include <memo/Exception.h>
#include <QSqlError>
#include <QSqlQuery>

class SqlDatabaseError : Memo::Exception {

public:
    SqlDatabaseError(const QSqlError& error);
    QString error() const override;

private:
    QSqlError m_error;
};

class SqlQueryError : Memo::Exception {

public:
    SqlQueryError(const QSqlQuery& query);
    QString error() const override;

private:
    QSqlQuery query;
};
