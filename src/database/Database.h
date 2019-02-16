#pragma once
#include <QObject>

class Database : public QObject {
    Q_OBJECT
public:
    explicit Database(QObject* parent = nullptr);

signals:

public slots:
};
