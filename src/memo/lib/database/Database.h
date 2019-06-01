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

    void create(const QString& filepath);
    void open(const QString& filepath);
    void close();
    bool isOpen() const;

    int insertRecord(int parentId, int pos, int depth, const QString& title);
    void removeRecord(int id);
    QSqlQuery record(int id);

    void updateValue(int id, const QString& name, const QVariant& value);
    QVariant value(int id, const QString& name);

    void updateMetaValue(const QString& name, const QVariant& value);
    QVariant metaValue(const QString& name);

    QVector<Title> titles();

private:
    QSqlDatabase m_db;
};
