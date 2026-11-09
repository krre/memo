#pragma once
#include <QObject>

class Settings;
class NativeEventFilter;

class GlobalHotkey : public QObject {
    Q_OBJECT
public:
    GlobalHotkey(QObject* parent = nullptr);

    void setShortcut(const QString& shortcut);
    void unsetShortcut();

    void applySettings(Settings* settings);

signals:
    void activated();

private:
    NativeEventFilter* m_nativeEventFilter = nullptr;
};
