#include "GlobalHotkey.h"
#include "NativeEventFilter.h"
#include <QCoreApplication>
#include <QKeySequence>

GlobalHotkey::GlobalHotkey(QObject* parent) : QObject(parent) {
    nativeEventFilter = new NativeEventFilter(this);
    connect(nativeEventFilter, &NativeEventFilter::activated, this, &GlobalHotkey::activated);
    qApp->installNativeEventFilter(nativeEventFilter);
}

void GlobalHotkey::setShortcut(const QString& shortcut) {
    if (shortcut.isEmpty()) {
        nativeEventFilter->unsetShortcut();
    } else {
        QKeySequence ks = QKeySequence(shortcut);
        Qt::Key key = Qt::Key(ks[0] & ~Qt::KeyboardModifierMask);
        Qt::KeyboardModifiers modifiers = Qt::KeyboardModifiers(ks[0] & Qt::KeyboardModifierMask);
        nativeEventFilter->setShortcut(key, modifiers);
    }
}

void GlobalHotkey::unsetShortcut() {
    nativeEventFilter->unsetShortcut();
}
