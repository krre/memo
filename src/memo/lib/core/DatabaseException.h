#pragma once
#include <common/Exception.h>
#include <QSqlError>
#include <QSqlQuery>

class DatabaseError : public Common::Exception {

public:
    DatabaseError(const QSqlError& error);
    QString error() const override;

private:
    QSqlError m_error;
};

class SqlQueryError : public Common::Exception {

public:
    SqlQueryError(const QSqlQuery& query);
    QString error() const override;

private:
    QSqlQuery m_query;
};
