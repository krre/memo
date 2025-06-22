#pragma once
#include <QObject>

class NativeEventFilter;

class GlobalHotkey : public QObject {
    Q_OBJECT
public:
    GlobalHotkey(QObject* parent = nullptr);

    void setShortcut(const QString& shortcut);
    void unsetShortcut();

signals:
    void activated();

private:
    NativeEventFilter* m_nativeEventFilter = nullptr;
};
