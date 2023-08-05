#pragma once
#include "core/Globals.h"
#include <QVariantMap>
#include <QSqlDatabase>

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

    explicit Database(QObject* parent = nullptr);
    ~Database() override;

    void create(const QString& filepath);
    void open(const QString& filepath);
    void close();
    bool isOpen() const;

    Id insertNote(Id parentId, int pos, int depth, const QString& title) const;
    void removeNote(Id id) const;
    Note note(Id id) const;
    QSqlQuery exec(const QString& sql, const QVariantMap& params = QVariantMap()) const;

    void updateValue(Id id, const QString& name, const QVariant& value) const;
    QVariant value(Id id, const QString& name) const;

    void updateMetaValue(const QString& name, const QVariant& value) const;
    QVariant metaValue(const QString& name) const;

    QVector<Note> notes() const;
    QString name() const;

private:
    Note queryToNote(const QSqlQuery& query) const;

    QSqlDatabase m_db;
};
