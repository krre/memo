#pragma once
#include <QObject>
#include <QSqlDatabase>

class Database : public QObject {
    Q_OBJECT
public:
    explicit Database(QObject* parent = nullptr);
    ~Database() override;

    void create(const QString& filepath);
    void open(const QString& filepath);
    void close();

private:
    QSqlDatabase db;
};
