#pragma once
#include <QHttpServer>

class Database;

class HttpServer : public QObject {
public:
    HttpServer(Database* database, QObject* parent = nullptr);

    void start(quint16 port, const QString& key);
    void stop();

private:
    Database* m_database = nullptr;
    QHttpServer* m_httpServer = nullptr;

    QString handleName() const;
    QByteArray handleNotes() const;
};
