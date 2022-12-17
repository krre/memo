#pragma once
#include <QObject>
#include <QSqlDatabase>
#include <core/Globals.h>

class Database : public QObject {
    Q_OBJECT
public:
    struct Title {
        Id id;
        Id parentId;
        int pos;
        int depth;
        QString title;
    };

    explicit Database(QObject* parent = nullptr);
    ~Database() override;

    void create(const QString& filepath);
    void open(const QString& filepath);
    void close();
    bool isOpen() const;

    int insertNote(Id parentId, int pos, int depth, const QString& title);
    void removeNote(Id id);
    QSqlQuery note(Id id);
    void exec(const QString& sql);

    void updateValue(Id id, const QString& name, const QVariant& value);
    QVariant value(Id id, const QString& name);

    void updateMetaValue(const QString& name, const QVariant& value);
    QVariant metaValue(const QString& name);

    QVector<Title> titles();

private:
    QSqlDatabase m_db;
};
