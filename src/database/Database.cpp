#include "Database.h"
#include "Migrater.h"
#include "DatabaseException.h"
#include <QFileInfo>

constexpr auto BirthdayDateFormat = "yyyy-MM-dd";

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

    Migrater migrater(this);
    migrater.run();
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

QSqlQuery Database::exec(const QString& sql, const QVariantMap& params) const {
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

Id Database::insertNote(Id parentId, int pos, int depth, const QString& title) const {
    QVariantMap params = {
        { "parent_id", parentId.value() },
        { "pos", pos },
        { "depth", depth },
        { "title", title }
    };

    QSqlQuery query = exec("INSERT INTO notes (parent_id, pos, depth, title) VALUES (:parent_id, :pos, :depth, :title)", params);
    return query.lastInsertId().toLongLong();
}

void Database::removeNote(Id id) const {
    exec("DELETE FROM notes WHERE id = :id", { { "id", id.value() } });
}

Note Database::note(Id id) const {
    QSqlQuery query = exec("SELECT * FROM notes WHERE id = :id", { { "id", id.value() } });
    query.next();
    return queryToNote(query);
}

QVector<Note> Database::notes() const {
    QVector<Note> result;
    QSqlQuery query = exec("SELECT * FROM notes ORDER BY depth, pos");

    while (query.next()) {
        result.append(queryToNote(query));
    }

    return result;
}

void Database::updateNoteValue(Id id, const QString& name, const QVariant& value) const {
    QVariantMap params = {
        { "id", id.value() },
        { "value", value },
    };

    QString updateDate = name == "note" ? ", updated_at = datetime('now', 'localtime')" : "";
    exec(QString("UPDATE notes SET %1 = :value %2 WHERE id = :id").arg(name, updateDate), params);
}

QVariant Database::noteValue(Id id, const QString& name) const {
    QSqlQuery query = exec(QString("SELECT %1 FROM notes WHERE id = :id").arg(name), { { "id", id.value() } });
    return query.first() ? query.value(name) : QVariant();
}

Id Database::insertBirthday(const Birthday& birthday) const {
    QVariantMap params = {
        { "date", birthday.date.toString(BirthdayDateFormat) },
        { "name", birthday.name }
    };

    QSqlQuery query = exec("INSERT INTO birthdays (date, name) VALUES (:date, :name)", params);
    return query.lastInsertId().toLongLong();
}

void Database::updateBirthday(const Birthday& birthday) const {
    QVariantMap params = {
        { "id", birthday.id.value() },
        { "date", birthday.date.toString(BirthdayDateFormat) },
        { "name", birthday.name }
    };

    exec(QString("UPDATE birthdays SET date = :date, name = :name WHERE id = :id"), params);
}

void Database::removeBirthday(Id id) const {
    exec("DELETE FROM birthdays WHERE id = :id", { { "id", id.value() } });
}

QVector<Birthday> Database::birthdays(const QDate& date) const {
    QVector<Birthday> result;
    QVariantMap params;
    QString where;

    if (!date.isNull()) {
        where = "WHERE strftime('%m-%d', date) = :date";
        params["date"] = date.toString("MM-dd");
    }

    QSqlQuery query = exec(QString("SELECT * FROM birthdays %1 ORDER BY strftime('%m-%d', date) ASC").arg(where), params);

    while (query.next()) {
        Birthday birthday;
        birthday.id = query.value("id").toInt();
        birthday.date = QDate::fromString(query.value("date").toString(), BirthdayDateFormat);
        birthday.name = query.value("name").toString();

        result.append(birthday);
    }

    return result;
}

bool Database::isBirthdayToday() const {
    QVariantMap params = {
        { "date", QDate::currentDate().toString("MM-dd") },
    };

    QSqlQuery query = exec("SELECT COUNT(*) FROM birthdays WHERE strftime('%m-%d', date) = :date", params);
    query.first();
    return query.value(0).toInt();
}

void Database::updateMetaValue(const QString& name, const QVariant& value) const {
    exec(QString("UPDATE meta SET %1 = :value").arg(name), { { "value", value } });
}

QVariant Database::metaValue(const QString& name) const {
    QSqlQuery query = exec(QString("SELECT %1 FROM meta").arg(name));
    return query.first() ? query.value(name) : QVariant();
}

QVector<FindNote> Database::find(const QString& text) const {
    QSqlQuery query = exec(QString("SELECT id, title, note FROM notes"));
    QVector<FindNote> result;

    while (query.next()) {
        Id id = query.value("id").toInt();
        QString title = query.value("title").toString();
        QString note = query.value("note").toString();

        if (title.contains(text, Qt::CaseInsensitive) || note.contains(text, Qt::CaseInsensitive)) {
            FindNote findNote;
            findNote.id = id;
            findNote.title = parentPath(id);

            result.append(findNote);
        }
    }

    return result;
}

QString Database::name() const {
    QFileInfo fi(m_db.databaseName());
    return fi.baseName();
}

Note Database::queryToNote(const QSqlQuery& query) const {
    Note result;
    result.id = query.value("id").toLongLong();
    result.parentId = query.value("parent_id").toInt();
    result.pos = query.value("pos").toInt();
    result.depth = query.value("depth").toInt();
    result.title = query.value("title").toString();
    result.note = query.value("note").toString();
    result.createdAt = query.value("created_at").toString();
    result.updatedAt = query.value("updated_at").toString();
    result.markdown = query.value("markdown").toInt();

    return result;
}

QString Database::parentPath(Id id) const {
    Id parentId = id;
    QStringList titles;

    while (parentId.isValid()) {
        QSqlQuery query = exec("SELECT title, parent_id FROM notes WHERE id = :id", { { "id", parentId.value() } });

        if (!query.next()) {
            break;
        }

        parentId = query.value("parent_id").toInt();
        titles.append(query.value("title").toString());
    }

    QString result;

    for (const auto& title : titles) {
        result.prepend(title + " > ");
    }

    result.chop(3);
    return result;
}
