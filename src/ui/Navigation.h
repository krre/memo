#pragma once
#include "core/Id.h"
#include <QObject>
#include <QList>

class Navigation : public QObject {
    Q_OBJECT
public:
    Navigation(QObject* parent = nullptr);

    void remove(Id id);

signals:
    void navigate(Id id);

    void backAvailable(bool backAvailable);
    void forwardAvailable(bool backAvailable);

public slots:
    void go(Id id);

    void back();
    void forward();

private:
    QList<Id> m_history;
    int m_position = -1;
    bool navigation = false;
};
