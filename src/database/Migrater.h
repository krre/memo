#pragma once
#include <QVector>

class Database;

class Migrater {
public:
    Migrater(Database* db);
    void run() const;

private:
    struct Migration {
        int version;
        std::function<void()> fn;
    };

    void setVersion(int version) const;

    void migration20190216() const;
    void migration20191214() const;
    void migration20231209() const;
    void migration20241024() const;

    Database* m_db = nullptr;
    QVector<Migration> m_migrations;
};
