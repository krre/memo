#pragma once
#include <QWidget>
#include <QtTest/QTest>

template <typename T>
T* child(QWidget* parent, const char* name) {
    auto widget = parent->findChild<T*>(name);
    Q_ASSERT_X(widget, "child", name);
    return widget;
}
