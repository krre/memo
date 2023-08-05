#pragma once
#include <QHash>

class Database;

class Patcher {

public:
    Patcher(Database* db);
    void run() const;

private:
    void patch2() const; // 14.12.2019

    Database* m_db;
    QHash<int, std::function<void()>> patches;
};
