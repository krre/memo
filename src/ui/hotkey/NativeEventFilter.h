#pragma once
#include <QObject>
#include <QAbstractNativeEventFilter>

class NativeEventFilter : public QObject, public QAbstractNativeEventFilter {
    Q_OBJECT

public:
    NativeEventFilter(QObject* parent = nullptr);

    bool nativeEventFilter(const QByteArray& eventType, void* message, long* result) override;

    void setShortcut(int keycode, unsigned int modifiers);
    void unsetShortcut();

signals:
    void activated();

private:
    int keycode;
    unsigned int modifiers;
};
