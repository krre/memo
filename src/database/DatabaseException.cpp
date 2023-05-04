#include "DatabaseException.h"

DatabaseError::DatabaseError(const QSqlError& error) : m_error(error) {

}

QString DatabaseError::error() const {
    return QString("Database error: %1. %2").arg(m_error.databaseText(), m_error.driverText());
}

SqlQueryError::SqlQueryError(const QSqlQuery& query) {
    m_error = QString("SQL query error: %1. %2. %3").arg(query.lastError().databaseText(), query.lastError().driverText(), query.lastQuery());
}

QString SqlQueryError::error() const {
    return m_error;
}
