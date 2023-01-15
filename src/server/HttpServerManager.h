#pragma once
#include <QHttpServer>

class Database;
class SolidString;

class HttpServerManager : public QObject {
public:
    HttpServerManager(Database* database, QObject* parent = nullptr);

    void start(quint16 port, const SolidString& token, const SolidString& certificatePath, const SolidString& privateKeyPath);
    void stop();

private:
    Database* m_database = nullptr;
    QHttpServer* m_httpServer = nullptr;
};
