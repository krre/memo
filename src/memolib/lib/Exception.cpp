#include "Exception.h"

namespace MemoLib {

const char* Exception::what() const noexcept {
    return text().toStdString().c_str();
}

RuntimeError::RuntimeError(const QString& error) : error(error) {

}

QString RuntimeError::text() const {
    return error;
}

} // MemoLib
