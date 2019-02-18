#pragma once
#include <QObject>
#include <QSqlDatabase>

class Database : public QObject {
    Q_OBJECT
public:
    struct Note {
        int id;
        int parent;
        int pos;
        QString title;
        QString note;
    };

    explicit Database(QObject* parent = nullptr);
    ~Database() override;

    bool create(const QString& filepath);
    bool open(const QString& filepath);
    void close();

    int insertRecord(int parent, int pos, const QString& title);
    QVector<Note> records() const;

private:
    void databaseError(const QSqlError& error);
    void queryError(const QSqlQuery& query);

    QSqlDatabase db;
};
