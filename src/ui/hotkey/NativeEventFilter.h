#pragma once
#include <QObject>
#include <QAbstractNativeEventFilter>

class NativeEventFilter : public QObject, public QAbstractNativeEventFilter {
    Q_OBJECT
public:
    NativeEventFilter(QObject* parent = nullptr);

    bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;

    void setShortcut(int key, uint mods);
    void unsetShortcut();

signals:
    void activated();

private:
    int m_key = 0;
    uint m_mods = 0;
};
