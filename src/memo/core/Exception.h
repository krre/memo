#pragma once
#include <stdexcept>
#include <QSqlError>
#include <QSqlQuery>

class Exception : std::exception {

public:
    Exception() = default;
    virtual QString text() const = 0;
};

class SqlDatabaseError : Exception {

public:
    SqlDatabaseError(const QSqlError& error);
    QString text() const override;

private:
    QSqlError error;
};

class SqlQueryError : Exception {

public:
    SqlQueryError(const QSqlQuery& query);
    QString text() const override;

private:
    QSqlQuery query;
};
