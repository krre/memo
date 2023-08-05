#include "Database.h"
#include "Patcher.h"
#include "DatabaseException.h"
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
        throw DatabaseError(m_db.lastError());
    }

    exec(
        "CREATE TABLE notes("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "parent_id INTEGER,"
            "pos INTEGER,"
            "depth INTEGER,"
            "title TEXT,"
            "note TEXT,"
            "created_at TIMESTAMP DEFAULT (datetime('now', 'localtime')),"
            "updated_at TIMESTAMP DEFAULT (datetime('now', 'localtime'))"
        ")"
    );

    exec("CREATE TABLE meta(version INTEGER, selected_id INTEGER)");
    exec("INSERT INTO meta (version, selected_id) VALUES (1, 0)");
}

void Database::open(const QString& filepath) {
    qInfo().noquote() << "Open database:" << filepath;
    m_db.setDatabaseName(filepath);

    if (!m_db.open()) {
        throw DatabaseError(m_db.lastError());
    }

    Patcher patcher(this);
    patcher.run();
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

int Database::insertNote(Id parentId, int pos, int depth, const QString& title) {
    QVariantMap params = {
        { "parent_id", parentId },
        { "pos", pos },
        { "depth", depth },
        { "title", title }
    };

    QSqlQuery query = exec("INSERT INTO notes (parent_id, pos, depth, title) VALUES (:parent_id, :pos, :depth, :title)", params);
    return query.lastInsertId().toInt();
}

void Database::removeNote(Id id) {
    exec("DELETE FROM notes WHERE id = :id", { { "id", id } });
}

Database::Note Database::note(Id id) {
    QSqlQuery query = exec("SELECT * FROM notes WHERE id = :id", { { "id", id } });
    query.next();
    return queryToNote(query);
}

QSqlQuery Database::exec(const QString& sql, const QVariantMap& params) {
    QSqlQuery query;
    query.prepare(sql);

    for (auto it = params.cbegin(); it != params.cend(); it++) {
        query.bindValue(":" + it.key(), it.value());
    }

    if (!query.exec()) {
        throw SqlQueryError(query);
    }

    return query;
}

void Database::updateValue(Id id, const QString& name, const QVariant& value) {
    QVariantMap params = {
        { "id", id },
        { "value", value },
    };

    QString updateDate = name == "note" ? ", updated_at = datetime('now', 'localtime')" : "";
    exec(QString("UPDATE notes SET %1 = :value %2 WHERE id = :id").arg(name, updateDate), params);
}

QVariant Database::value(Id id, const QString& name) {
    QSqlQuery query = exec(QString("SELECT %1 FROM notes WHERE id = :id").arg(name), { { "id", id } });
    return query.first() ? query.value(name) : QVariant();
}

void Database::updateMetaValue(const QString& name, const QVariant& value) {
    exec(QString("UPDATE meta SET %1 = :value").arg(name), { { "value", value } });
}

QVariant Database::metaValue(const QString& name) {
    QSqlQuery query = exec(QString("SELECT %1 FROM meta").arg(name));
    return query.first() ? query.value(name) : QVariant();
}

QVector<Database::Note> Database::notes() {
    QVector<Note> result;
    QSqlQuery query = exec("SELECT * FROM notes ORDER BY depth, pos");

    while (query.next()) {
        result.append(queryToNote(query));
    }

    return result;
}

QString Database::name() const {
    QFileInfo fi(m_db.databaseName());
    return fi.baseName();
}

Database::Note Database::queryToNote(const QSqlQuery& query) const {
    Note result;
    result.id = query.value("id").toInt();
    result.parentId = query.value("parent_id").toInt();
    result.pos = query.value("pos").toInt();
    result.depth = query.value("depth").toInt();
    result.title = query.value("title").toString();
    result.note = query.value("note").toString();
    result.createdAt = query.value("created_at").toString();
    result.updatedAt = query.value("updated_at").toString();

    return result;
}
