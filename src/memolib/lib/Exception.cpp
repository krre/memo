#include "Exception.h"

namespace MemoLib {

const char* Exception::what() const noexcept {
    return error().toStdString().c_str();
}

RuntimeError::RuntimeError(const QString& error) : m_error(error) {

}

QString RuntimeError::error() const {
    return m_error;
}

} // MemoLib
