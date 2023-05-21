#include "Patcher.h"
#include "Database.h"
#include <QVariant>

const int currentVersion = 2;

Patcher::Patcher(Database* db) : m_db(db) {
    patches[2] = [this] { patch2(); };
}

void Patcher::run() {
    int version = m_db->metaValue("version").toInt();

    if (version == currentVersion) return;

    for (int i = version + 1; i <= currentVersion; ++i) {
        patches[i]();
        qInfo() << "Run database patch" << i;
    }

    m_db->updateMetaValue("version", currentVersion);
}

void Patcher::patch2() {
    m_db->exec("ALTER TABLE notes ADD COLUMN line INTEGER");
}
