#pragma once
#include <QHash>

class Database;

class Migrater {
public:
    Migrater(Database* db);
    void run() const;

private:
    void migration1() const; // 16.02.2019
    void migration2() const; // 14.12.2019
    void migration3() const; // 09.12.2023
    void migration4() const; // 24.10.2024

    Database* m_db = nullptr;
    QHash<int, std::function<void()>> migrations;
};
