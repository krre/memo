#pragma once
#include <QVariantMap>
#include <QSqlDatabase>
#include "core/Globals.h"

class Database : public QObject {
    Q_OBJECT
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

    int insertNote(Id parentId, int pos, int depth, const QString& title);
    void removeNote(Id id);
    Note note(Id id);
    QSqlQuery exec(const QString& sql, const QVariantMap& params = QVariantMap());

    void updateValue(Id id, const QString& name, const QVariant& value);
    QVariant value(Id id, const QString& name);

    void updateMetaValue(const QString& name, const QVariant& value);
    QVariant metaValue(const QString& name);

    QVector<Note> notes();
    QString name() const;

private:
    Note queryToNote(const QSqlQuery& query) const;

    QSqlDatabase m_db;
};
