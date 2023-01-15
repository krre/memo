#include "SolidString.h"
#include "Exception.h"

SolidString::SolidString(const QString& other) : QString(other) {
    if (other.isEmpty()) {
        throw RuntimeError("String is empty");
    }
}
