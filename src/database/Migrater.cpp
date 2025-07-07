#include "Migrater.h"
#include "Database.h"
#include <QSqlQuery>

constexpr auto CurrentVersion = 4;

Migrater::Migrater(Database* db) : m_db(db) {
    migrations[1] = [this] { migration1(); };
    migrations[2] = [this] { migration2(); };
    migrations[3] = [this] { migration3(); };
    migrations[4] = [this] { migration4(); };
}

void Migrater::run() const {
    int version = m_db->metaValue("version").toInt();

    if (version == CurrentVersion) return;

    for (int i = version + 1; i <= CurrentVersion; ++i) {
        migrations[i]();
        qInfo() << "Run database migration:" << i;
    }

    m_db->updateMetaValue("version", CurrentVersion);
}

void Migrater::migration1() const {
    m_db->exec(R"(
        CREATE TABLE notes(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            parent_id INTEGER,
            pos INTEGER,
            depth INTEGER,
            title TEXT,
            note TEXT,
            created_at TIMESTAMP DEFAULT (datetime('now', 'localtime')),
            updated_at TIMESTAMP DEFAULT (datetime('now', 'localtime'))
        ))"
    );

    m_db->exec(R"(
        CREATE TABLE meta(
            version INTEGER,
            selected_id INTEGER
        ))"
    );

    m_db->exec("INSERT INTO meta (version, selected_id) VALUES (1, 0)");
}

void Migrater::migration2() const {
    m_db->exec("ALTER TABLE notes ADD COLUMN line INTEGER");
}

void Migrater::migration3() const {
    m_db->exec(R"(
        CREATE TABLE birthdays(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            date DATE,
            name TEXT
        ))"
    );
}

void Migrater::migration4() const {
    m_db->exec("ALTER TABLE notes ADD COLUMN markdown BOOLEAN NOT NULL DEFAULT 0");
}
