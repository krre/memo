#include "Exception.h"

const char* Exception::what() const noexcept {
    m_errorString = error().toStdString();
    return m_errorString.c_str();
}

RuntimeError::RuntimeError(const QString& error) : m_error(error) {

}

QString RuntimeError::error() const {
    return m_error;
}
