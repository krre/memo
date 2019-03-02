#include "NativeEventFilter.h"
#include <QtX11Extras/QX11Info>
#include <QDebug>
#include <QMetaMethod>
#include <xcb/xcb.h>
#include <X11/Xutil.h>

namespace  {
    Display* display;
    Window win;
    int keycode;
    unsigned int modifier;

    QVector<quint32> maskModifiers(){
        return QVector<quint32>() << 0 << Mod2Mask << LockMask << (Mod2Mask | LockMask);
    }
}

NativeEventFilter::NativeEventFilter(QObject* parent) : QObject(parent) {
    display = QX11Info::display();
    win = DefaultRootWindow(display);
}

bool NativeEventFilter::nativeEventFilter(const QByteArray& eventType, void* message, long* result) {
    Q_UNUSED(result)

    if (eventType == "xcb_generic_event_t") {
        xcb_generic_event_t* event = static_cast<xcb_generic_event_t *>(message);

        if ((event->response_type & 127) == XCB_KEY_PRESS){
            xcb_key_press_event_t* keyEvent = static_cast<xcb_key_press_event_t*>(message);

            for (quint32 maskMods : maskModifiers()) {
                if((keyEvent->state == (modifier | maskMods )) &&  keyEvent->detail == keycode) {
                    emit activated();
                    return true;
                }
            }
        }
    }

    return false;
}

void NativeEventFilter::setShortcut(const QKeySequence& shortcut) {
    keycode = XKeysymToKeycode(display, XK_M);
    modifier = ControlMask | Mod1Mask;

    for (quint32 maskMods : maskModifiers()) {
        XGrabKey(display, keycode, modifier | maskMods, win, True, GrabModeAsync, GrabModeAsync);
    }
}

void NativeEventFilter::unsetShortcut() {
    for(quint32 maskMods : maskModifiers()) {
        XUngrabKey(display, keycode, modifier | maskMods, win);
    }
}
