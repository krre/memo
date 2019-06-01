#include "Database.h"
#include "core/SqlException.h"
#include <QtSql>

Database::Database(QObject* parent) : QObject(parent) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
}

Database::~Database() {
    close();
}

void Database::create(const QString& filepath) {
    qInfo().noquote() << "Create database:" << filepath;
    m_db.close();
    m_db.setDatabaseName(filepath);

    if (!m_db.open()) {
        throw SqlDatabaseError(m_db.lastError());
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE notes("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "parent_id INTEGER,"
                    "pos INTEGER,"
                    "depth INTEGER,"
                    "title TEXT,"
                    "note TEXT,"
                    "created_at TIMESTAMP DEFAULT (datetime('now', 'localtime')),"
                    "updated_at TIMESTAMP DEFAULT (datetime('now', 'localtime')))")) {
        throw SqlQueryError(query);
    }

    if (!query.exec("CREATE TABLE meta("
                    "version INTEGER,"
                    "selected_id INTEGER)")) {
        throw SqlQueryError(query);
    }

    if (!query.exec("INSERT INTO meta (version, selected_id) VALUES (1, 0)")) {
        throw SqlQueryError(query);
    }
}

void Database::open(const QString& filepath) {
    qInfo().noquote() << "Open database:" << filepath;
    m_db.setDatabaseName(filepath);

    if (!m_db.open()) {
        throw SqlDatabaseError(m_db.lastError());
    }
}

void Database::close() {
    if (m_db.isOpen()) {
        qInfo().noquote() << "Close database:" << m_db.databaseName();
        m_db.close();
    }
}

bool Database::isOpen() const {
    return m_db.isOpen();
}

int Database::insertRecord(int parentId, int pos, int depth, const QString& title) {
    QSqlQuery query;
    query.prepare("INSERT INTO notes (parent_id, pos, depth, title) VALUES (:parent_id, :pos, :depth, :title)");
    query.bindValue(":parent_id", parentId);
    query.bindValue(":pos", pos);
    query.bindValue(":depth", depth);
    query.bindValue(":title", title);

    if (!query.exec()) {
        throw SqlQueryError(query);
    }

    return query.lastInsertId().toInt();
}

void Database::removeRecord(int id) {
    QSqlQuery query;
    query.prepare("DELETE FROM notes WHERE id = :id");
    query.bindValue(":id", id);

    if (!query.exec()) {
        throw SqlQueryError(query);
    }
}

QSqlQuery Database::record(int id) {
    QSqlQuery query;
    query.prepare(QString("SELECT * FROM notes WHERE id = :id"));
    query.bindValue(":id", id);

    if (!query.exec()) {
        throw SqlQueryError(query);
    } else {
        query.next();
    }

    return query;
}

void Database::updateValue(int id, const QString& name, const QVariant& value) {
    QSqlQuery query;
    QString updateDate = name == "note" ? ", updated_at = datetime('now', 'localtime')" : "";
    query.prepare(QString("UPDATE notes SET %1 = :value %2 WHERE id = :id").arg(name, updateDate));
    query.bindValue(":id", id);
    query.bindValue(":value", value);

    if (!query.exec()) {
        throw SqlQueryError(query);
    }
}

QVariant Database::value(int id, const QString& name) {
    QSqlQuery query;
    query.prepare(QString("SELECT %1 FROM notes WHERE id = :id").arg(name));
    query.bindValue(":id", id);

    if (!query.exec()) {
        throw SqlQueryError(query);
    }

    if (query.first()) {
        return query.value(name);
    }

    return QVariant();
}

void Database::updateMetaValue(const QString& name, const QVariant& value) {
    QSqlQuery query;
    query.prepare(QString("UPDATE meta SET %1 = :value").arg(name));
    query.bindValue(":value", value);

    if (!query.exec()) {
        throw SqlQueryError(query);
    }
}

QVariant Database::metaValue(const QString& name) {
    QSqlQuery query;
    query.prepare(QString("SELECT %1 FROM meta").arg(name));

    if (!query.exec()) {
        throw SqlQueryError(query);
    }

    if (query.first()) {
        return query.value(name);
    }

    return QVariant();
}

QVector<Database::Title> Database::titles() {
    QVector<Title> list;

    QSqlQuery query("SELECT id, parent_id, pos, depth, title FROM notes ORDER BY depth, pos");

    while (query.next()) {
        Title title;
        title.id = query.value("id").toInt();
        title.parentId = query.value("parent_id").toInt();
        title.pos = query.value("pos").toInt();
        title.depth = query.value("depth").toInt();
        title.title = query.value("title").toString();

        list.append(title);
    }

    return list;
}
