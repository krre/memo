#pragma once
#include <QHash>

class Database;

class Patcher {

public:
    Patcher(Database* db);
    void run() const;

private:
    void patch2() const; // 14.12.2019
    void patch3() const; // 09.12.2023

    Database* m_db = nullptr;
    QHash<int, std::function<void()>> patches;
};
