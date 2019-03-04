#include "NativeEventFilter.h"
#include <QtX11Extras/QX11Info>
#include <QKeySequence>
#include <QVector>
#include <xcb/xcb.h>
#include <X11/Xutil.h>

namespace  {
    Display* display;
    Window win;
}

const QVector<quint32> maskModifiers = { 0, Mod2Mask, LockMask, (Mod2Mask | LockMask) };

NativeEventFilter::NativeEventFilter(QObject* parent) : QObject(parent) {
    display = QX11Info::display();
    win = DefaultRootWindow(display);
}

bool NativeEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, long* result) {
    Q_UNUSED(result)

    if (eventType == "xcb_generic_event_t") {
        xcb_generic_event_t* event = static_cast<xcb_generic_event_t *>(message);

        if ((event->response_type & 127) == XCB_KEY_PRESS) {
            xcb_key_press_event_t* keyEvent = static_cast<xcb_key_press_event_t*>(message);

            for (quint32 maskMods : maskModifiers) {
                if ((keyEvent->state == (mods | maskMods )) && keyEvent->detail == key) {
                    emit activated();
                    return true;
                }
            }
        }
    }

    return false;
}

void NativeEventFilter::setShortcut(int keycode, unsigned int modifiers) {
    mods = 0;

    if (modifiers & Qt::ShiftModifier) mods |= ShiftMask;
    if (modifiers & Qt::ControlModifier) mods |= ControlMask;
    if (modifiers & Qt::AltModifier) mods |= Mod1Mask;
    if (modifiers & Qt::MetaModifier) mods |= Mod4Mask;

    QString keyString = QKeySequence(keycode).toString(QKeySequence::NativeText);
    KeySym keysym = XStringToKeysym(keyString.toLatin1().constData());

    key = XKeysymToKeycode(display, keysym);

    for (quint32 maskMods : maskModifiers) {
        XGrabKey(display, key, mods | maskMods, win, True, GrabModeAsync, GrabModeAsync);
    }
}

void NativeEventFilter::unsetShortcut() {
    for(quint32 maskMods : maskModifiers) {
        XUngrabKey(display, key, mods | maskMods, win);
    }
}