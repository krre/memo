#include "Migrater.h"
#include "Database.h"
#include <QSqlQuery>

constexpr auto currentVersion = 3;

Migrater::Migrater(Database* db) : m_db(db) {
    migrations[2] = [this] { migration2(); };
    migrations[3] = [this] { migration3(); };
}

void Migrater::run() const {
    int version = m_db->metaValue("version").toInt();

    if (version == currentVersion) return;

    for (int i = version + 1; i <= currentVersion; ++i) {
        migrations[i]();
        qInfo() << "Run database migration" << i;
    }

    m_db->updateMetaValue("version", currentVersion);
}

void Migrater::migration2() const {
    m_db->exec("ALTER TABLE notes ADD COLUMN line INTEGER");
}

void Migrater::migration3() const {
    m_db->exec(
        "CREATE TABLE birthdays("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "date DATE,"
            "name TEXT"
        ")"
    );
}
