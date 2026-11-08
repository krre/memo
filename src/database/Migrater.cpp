#include "Migrater.h"
#include "Database.h"
#include <QSqlQuery>

#define MIGRATION(ver) \
    m_migrations.append({ ver, [this] { migration##ver(); } })

Migrater::Migrater(Database* db) : m_db(db) {
    MIGRATION(20190216);
    MIGRATION(20191214);
    MIGRATION(20231209);
    MIGRATION(20241024);
}

void Migrater::run() const {
    const int dbVersion = m_db->metaValue("version").toInt();
    const int latestVersion = m_migrations.last().version;

    if (dbVersion >= latestVersion) return;

    // Backward compatibility
    if (dbVersion == 4) {
        setVersion(latestVersion);
        return;
    }

    for (const auto& migration: m_migrations) {
        if (migration.version <= dbVersion) continue;

        qInfo() << "Run database migration:" << migration.version;
        migration.fn();
        setVersion(migration.version);
    }
}

void Migrater::setVersion(int version) const {
    m_db->updateMetaValue("version", version);
}

void Migrater::migration20190216() const {
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

void Migrater::migration20191214() const {
    m_db->exec("ALTER TABLE notes ADD COLUMN line INTEGER");
}

void Migrater::migration20231209() const {
    m_db->exec(R"(
        CREATE TABLE birthdays(
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            date DATE,
            name TEXT
        ))"
    );
}

void Migrater::migration20241024() const {
    m_db->exec("ALTER TABLE notes ADD COLUMN markdown BOOLEAN NOT NULL DEFAULT 0");
}
