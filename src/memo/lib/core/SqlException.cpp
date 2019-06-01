#include "SqlException.h"

SqlDatabaseError::SqlDatabaseError(const QSqlError& error) : m_error(error) {

}

QString SqlDatabaseError::error() const {
    return QString("Database error: %1. %2").arg(m_error.databaseText(), m_error.driverText());
}

SqlQueryError::SqlQueryError(const QSqlQuery& query) : query(query) {

}

QString SqlQueryError::error() const {
    return QString("SQL query error: %1. %2. %3").arg(query.lastError().databaseText(), query.lastError().driverText(), query.lastQuery());
}
