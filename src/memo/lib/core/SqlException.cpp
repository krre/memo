#include "SqlException.h"

DatabaseError::DatabaseError(const QSqlError& error) : m_error(error) {

}

QString DatabaseError::error() const {
    return QString("Database error: %1. %2").arg(m_error.databaseText(), m_error.driverText());
}

SqlQueryError::SqlQueryError(const QSqlQuery& query) : m_query(query) {

}

QString SqlQueryError::error() const {
    return QString("SQL query error: %1. %2. %3").arg(m_query.lastError().databaseText(), m_query.lastError().driverText(), m_query.lastQuery());
}
