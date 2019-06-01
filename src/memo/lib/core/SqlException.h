#pragma once
#include <common/Exception.h>
#include <QSqlError>
#include <QSqlQuery>

class SqlDatabaseError : Common::Exception {

public:
    SqlDatabaseError(const QSqlError& error);
    QString error() const override;

private:
    QSqlError m_error;
};

class SqlQueryError : Common::Exception {

public:
    SqlQueryError(const QSqlQuery& query);
    QString error() const override;

private:
    QSqlQuery query;
};
