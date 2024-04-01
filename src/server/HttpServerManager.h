#pragma once
#include <QObject>

class QHttpServer;
class Database;
class SolidString;

class HttpServerManager : public QObject {
public:
    HttpServerManager(Database* database, QObject* parent = nullptr);

    void start(quint16 port, const SolidString& token, const SolidString& certificatePath, const SolidString& privateKeyPath);
    void start(quint16 port, const SolidString& token);

    void stop();

private:
    void startImpl(quint16 port, const SolidString& token);

    Database* m_database = nullptr;
    QHttpServer* m_httpServer = nullptr;
};
