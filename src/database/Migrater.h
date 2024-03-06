#pragma once
#include <QHash>

class Database;

class Migrater {
public:
    Migrater(Database* db);
    void run() const;

private:
    void migration2() const; // 14.12.2019
    void migration3() const; // 09.12.2023

    Database* m_db = nullptr;
    QHash<int, std::function<void()>> migrations;
};
