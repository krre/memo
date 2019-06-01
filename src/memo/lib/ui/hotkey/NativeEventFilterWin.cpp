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

void NativeEventFilter::setShortcut(int key, uint mods) {
    m_mods = 0;

    if (mods & Qt::ShiftModifier) m_mods |= MOD_SHIFT;
    if (mods & Qt::ControlModifier) m_mods |= MOD_CONTROL;
    if (mods & Qt::AltModifier) m_mods |= MOD_ALT;
    if (mods & Qt::MetaModifier) m_mods |= MOD_WIN;

    m_key = LOBYTE(VkKeyScanW(key));

    BOOL ok = RegisterHotKey(nullptr, HKEY_ID(m_key, m_mods), m_mods, m_key);

    if (!ok) {
        qWarning() << "Failed to register hotkey";
    }
}

void NativeEventFilter::unsetShortcut() {
    BOOL ok = UnregisterHotKey(nullptr, HKEY_ID(m_key, m_mods));

    if (!ok) {
        qWarning() << "Failed to unregister hotkey";
    }
}
