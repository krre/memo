#include "SqlException.h"

SqlDatabaseError::SqlDatabaseError(const QSqlError& error) : error(error) {

}

QString SqlDatabaseError::text() const {
    return QString("Database error: %1. %2").arg(error.databaseText(), error.driverText());
}

SqlQueryError::SqlQueryError(const QSqlQuery& query) : query(query) {

}

QString SqlQueryError::text() const {
    return QString("SQL query error: %1. %2. %3").arg(query.lastError().databaseText(), query.lastError().driverText(), query.lastQuery());
}
