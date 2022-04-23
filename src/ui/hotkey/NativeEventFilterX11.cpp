#include "NativeEventFilter.h"
#include <QKeySequence>
#include <QVector>
#include <QGuiApplication>
#include <xcb/xcb.h>
#include <X11/Xutil.h>

namespace {
    Display* display;
    Window win;
}

const quint32 maskModifiers[] = { 0, Mod2Mask, LockMask, (Mod2Mask | LockMask) };

NativeEventFilter::NativeEventFilter(QObject* parent) : QObject(parent) {
    const QNativeInterface::QX11Application *x11Interface = qGuiApp->nativeInterface<QNativeInterface::QX11Application>();
    display = x11Interface->display();
    win = DefaultRootWindow(display);
}

bool NativeEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, qintptr* result) {
    Q_UNUSED(result)

    if (eventType == "xcb_generic_event_t") {
        auto event = static_cast<xcb_generic_event_t*>(message);

        if ((event->response_type & 127) == XCB_KEY_PRESS) {
            auto keyEvent = static_cast<xcb_key_press_event_t*>(message);

            for (quint32 maskMods : maskModifiers) {
                if ((keyEvent->state == (m_mods | maskMods )) && keyEvent->detail == m_key) {
                    emit activated();
                    return true;
                }
            }
        }
    }

    return false;
}

void NativeEventFilter::setShortcut(int keycode, unsigned int modifiers) {
    m_mods = 0;

    if (modifiers & Qt::ShiftModifier) m_mods |= ShiftMask;
    if (modifiers & Qt::ControlModifier) m_mods |= ControlMask;
    if (modifiers & Qt::AltModifier) m_mods |= Mod1Mask;
    if (modifiers & Qt::MetaModifier) m_mods |= Mod4Mask;

    QString keyString = QKeySequence(keycode).toString(QKeySequence::NativeText);
    KeySym keysym = XStringToKeysym(keyString.toLatin1().constData());

    m_key = XKeysymToKeycode(display, keysym);

    for (quint32 maskMods : maskModifiers) {
        XGrabKey(display, m_key, m_mods | maskMods, win, True, GrabModeAsync, GrabModeAsync);
    }
}

void NativeEventFilter::unsetShortcut() {
    for(quint32 maskMods : maskModifiers) {
        XUngrabKey(display, m_key, m_mods | maskMods, win);
    }
}
