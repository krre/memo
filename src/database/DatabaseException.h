#pragma once
#include "core/Exception.h"
#include <QSqlError>
#include <QSqlQuery>

class DatabaseError : public Exception {
public:
    DatabaseError(const QSqlError& error);
    QString error() const override;

private:
    QSqlError m_error;
};

class SqlQueryError : public Exception {
public:
    SqlQueryError(const QSqlQuery& query);
    QString error() const override;

private:
    QString m_error;
};
