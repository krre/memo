#include "Exception.h"

namespace MemoLib {

RuntimeError::RuntimeError(const QString& error) : error(error) {

}

QString RuntimeError::text() const {
    return error;
}

} // MemoLib
