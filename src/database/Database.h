#pragma once
#include <QObject>
#include <QSqlDatabase>

class Database : public QObject {
    Q_OBJECT
public:
    struct Title {
        int id;
        int parentId;
        int pos;
        int depth;
        QString title;
    };

    explicit Database(QObject* parent = nullptr);
    ~Database() override;

    bool create(const QString& filepath);
    bool open(const QString& filepath);
    void close();
    bool isOpen() const;

    int insertRecord(int parentId, int pos, int depth, const QString& title);
    bool removeRecord(int id);

    bool updateValue(int id, const QString& name, const QVariant& value);
    QVariant value(int id, const QString& name);

    bool updateMetaValue(const QString& name, const QVariant& value);
    QVariant metaValue(const QString& name);

    QVector<Title> titles();

private:
    void databaseError(const QSqlError& error);
    void queryError(const QSqlQuery& query);

    QSqlDatabase db;
};
