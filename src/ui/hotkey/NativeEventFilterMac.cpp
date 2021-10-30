#include "NativeEventFilter.h"

NativeEventFilter::NativeEventFilter(QObject* parent) : QObject(parent) {

}

bool NativeEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) {
    Q_UNUSED(eventType)
    Q_UNUSED(message)
    Q_UNUSED(result)

    return false;
}

void NativeEventFilter::setShortcut(int keycode, unsigned int modifiers) {
    Q_UNUSED(keycode)
    Q_UNUSED(modifiers)
}

void NativeEventFilter::unsetShortcut() {

}
