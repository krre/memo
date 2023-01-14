#pragma once
#include <QHttpServer>

class Database;

class HttpServerManager : public QObject {
public:
    HttpServerManager(Database* database, QObject* parent = nullptr);

    void start(quint16 port, const QString& token, const QString& certificatePath, const QString& privateKeyPath);
    void stop();

private:
    Database* m_database = nullptr;
    QHttpServer* m_httpServer = nullptr;
};
