#include "GlobalHotkey.h"
#include "NativeEventFilter.h"
#include <QCoreApplication>
#include <QKeySequence>

GlobalHotkey::GlobalHotkey(QObject* parent) : QObject(parent) {
    m_nativeEventFilter = new NativeEventFilter(this);
    connect(m_nativeEventFilter, &NativeEventFilter::activated, this, &GlobalHotkey::activated);
    qApp->installNativeEventFilter(m_nativeEventFilter);
}

void GlobalHotkey::setShortcut(const QString& shortcut) {
    if (shortcut.isEmpty()) {
        m_nativeEventFilter->unsetShortcut();
    } else {
        QKeySequence ks = QKeySequence(shortcut);
        auto key = Qt::Key(ks[0] & ~Qt::KeyboardModifierMask);
        auto modifiers = Qt::KeyboardModifiers(ks[0] & Qt::KeyboardModifierMask);
        m_nativeEventFilter->setShortcut(key, modifiers);
    }
}

void GlobalHotkey::unsetShortcut() {
    m_nativeEventFilter->unsetShortcut();
}
