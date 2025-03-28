#include "SafeString.h"
#include "Exception.h"

SafeString::SafeString(const QString& other) : QString(other) {
    if (other.isEmpty()) {
        throw RuntimeError("String is empty");
    }
}
