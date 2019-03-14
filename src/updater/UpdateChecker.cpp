#include "UpdateChecker.h"

UpdateChecker::UpdateChecker(QObject* parent) : QObject(parent) {

}

void UpdateChecker::check() {
    emit checkResult({});
}
