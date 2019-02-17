#pragma once
#include <QObject>
#include <QSqlDatabase>

class Database : public QObject {
    Q_OBJECT
public:
    explicit Database(QObject* parent = nullptr);
    ~Database() override;

    bool create(const QString& filepath);
    bool open(const QString& filepath);
    void close();

private:
    void databaseError(const QSqlError& error);
    void queryError(const QSqlQuery& query);

    QSqlDatabase db;
};
