#pragma once
#include <QObject>

class Database : public QObject {
    Q_OBJECT
public:
    explicit Database(QObject* parent = nullptr);

    void create(const QString& filepath);
    void open(const QString& filepath);
    void close();

signals:

public slots:
};
