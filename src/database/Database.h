#pragma once
#include "core/Model.h"
#include <QVariantMap>
#include <QSqlDatabase>

class Database : public QObject {
public:
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
    bool isBirthdayToday() const;

    void updateMetaValue(const QString& name, const QVariant& value) const;
    QVariant metaValue(const QString& name) const;

    QVector<FindNote> find(const QString& text) const;

    QString name() const;

private:
    Note queryToNote(const QSqlQuery& query) const;
    QString parentPath(Id id) const;

    QSqlDatabase m_db;
};
