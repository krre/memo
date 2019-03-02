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
        nativeEventFilter->setShortcut(QKeySequence(shortcut));
    }
}

void GlobalHotkey::unsetShortcut() {
    nativeEventFilter->unsetShortcut();
}
