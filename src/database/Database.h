#pragma once
#include "core/Globals.h"
#include <QVariantMap>
#include <QSqlDatabase>
#include <QDate>

class Database : public QObject {

public:
    struct Note {
        Id id;
        Id parentId;
        int pos;
        int depth;
        QString title;
        QString note;
        QString createdAt;
        QString updatedAt;
    };

    struct Birthday {
        Id id;
        QDate date;
        QString name;
    };

    explicit Database(QObject* parent = nullptr);
    ~Database() override;

    void create(const QString& filepath);
    void open(const QString& filepath);
    void close();
    bool isOpen() const;

    QSqlQuery exec(const QString& sql, const QVariantMap& params = QVariantMap()) const;

    Id insertNote(Id parentId, int pos, int depth, const QString& title) const;
    void removeNote(Id id) const;
    Note note(Id id) const;
    QVector<Note> notes() const;

    void updateNoteValue(Id id, const QString& name, const QVariant& value) const;
    QVariant noteValue(Id id, const QString& name) const;

    Id insertBirthday(const Birthday& birthday) const;
    void updateBirthday(const Birthday& birthday) const;
    void removeBirthday(Id id) const;
    QVector<Birthday> birthdays(const QDate& date = QDate()) const;

    void updateMetaValue(const QString& name, const QVariant& value) const;
    QVariant metaValue(const QString& name) const;

    QString name() const;

private:
    Note queryToNote(const QSqlQuery& query) const;

    QSqlDatabase m_db;
};
