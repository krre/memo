#include "Database.h"
#include <QtSql>

Database::Database(QObject* parent) : QObject(parent) {
    db = QSqlDatabase::addDatabase("QSQLITE");
}

Database::~Database() {
    close();
}

bool Database::create(const QString& filepath) {
    qInfo().noquote() << "Create database:" << filepath;
    db.close();
    db.setDatabaseName(filepath);
    if (!db.open()) {
        databaseError(db.lastError());
        return false;
    }

    QSqlQuery query;
    if (!query.exec("CREATE TABLE notes("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                    "parent INTEGER,"
                    "pos INTEGER,"
                    "title TEXT,"
                    "note TEXT,"
                    "created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,"
                    "updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP"
                    ")")) {
        queryError(query);
        return false;
    }

    return true;
}

bool Database::open(const QString& filepath) {
    qDebug().noquote() << "Open database:" << filepath;
    db.close();
    db.setDatabaseName(filepath);
    if (!db.open()) {
        databaseError(db.lastError());
        return false;
    }

    return true;
}

void Database::close() {
    if (db.isOpen()) {
        qInfo().noquote() << "Close database:" << db.databaseName();
        db.close();
    }
}

int Database::insertRecord(int parent, int pos, const QString& title) {
    QSqlQuery query;
    query.prepare("INSERT INTO notes (parent, pos, title) VALUES (:parent, :pos, :title)");
    query.bindValue(":parent", parent + 1);
    query.bindValue(":pos", pos);
    query.bindValue(":title", title);

    if (!query.exec()) {
        queryError(query);
        return -1;
    }

    return query.lastInsertId().toInt();
}

QVector<Database::Note> Database::records() const {
    QVector<Note> notes;

    QSqlQuery query("SELECT id, parent, pos, title, note FROM notes");
    while (query.next()) {
        Note note;
        note.id = query.value("id").toInt();
        note.parent = query.value("parent").toInt();
        note.pos = query.value("pos").toInt();
        note.title = query.value("title").toString();
        note.note = query.value("note").toString();

        notes.append(note);
    }

    return notes;
}

void Database::databaseError(const QSqlError& error) {
    qCritical("Database error: %s. %s", qUtf8Printable(error.databaseText()), qUtf8Printable(error.driverText()));
}

void Database::queryError(const QSqlQuery& query) {
    qCritical("SQL query error: %s. %s. %s", qUtf8Printable(query.lastError().databaseText()), qUtf8Printable(query.lastError().driverText()), qUtf8Printable(query.lastQuery()));
}
