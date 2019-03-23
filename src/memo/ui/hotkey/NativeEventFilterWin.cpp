#include "NativeEventFilter.h"
#include <qt_windows.h>
#include <QDebug>

#define HKEY_ID(key, modifiers) (((key ^ (modifiers << 8)) & 0x0FFF) | 0x7000)

NativeEventFilter::NativeEventFilter(QObject* parent) : QObject(parent) {

}

bool NativeEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, long* result) {
    Q_UNUSED(eventType)
    Q_UNUSED(result)

    MSG* msg = static_cast<MSG*>(message);
    if (msg->message == WM_HOTKEY) {
        emit activated();
    }

    return false;
}

void NativeEventFilter::setShortcut(int keycode, unsigned int modifiers) {
    mods = 0;

    if (modifiers & Qt::ShiftModifier) mods |= MOD_SHIFT;
    if (modifiers & Qt::ControlModifier) mods |= MOD_CONTROL;
    if (modifiers & Qt::AltModifier) mods |= MOD_ALT;
    if (modifiers & Qt::MetaModifier) mods |= MOD_WIN;

    key = LOBYTE(VkKeyScanW(keycode));

    BOOL ok = RegisterHotKey(nullptr, HKEY_ID(key, mods), mods, key);

    if (!ok) {
        qWarning() << "Failed to register hotkey";
    }
}

void NativeEventFilter::unsetShortcut() {
    BOOL ok = UnregisterHotKey(nullptr, HKEY_ID(key, mods));

    if (!ok) {
        qWarning() << "Failed to unregister hotkey";
    }
}
