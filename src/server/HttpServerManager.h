#pragma once
#include <QObject>

class QHttpServer;
class QTcpServer;

class Settings;
class Database;
class SafeString;

class HttpServerManager : public QObject {
public:
    HttpServerManager(Database* database, QObject* parent = nullptr);

    void applySettings(Settings* settings);

private:
    void start(quint16 port, const SafeString& token, const SafeString& certificatePath, const SafeString& privateKeyPath);
    void start(quint16 port, const SafeString& token);

    void stop();

    void startImpl(quint16 port, const SafeString& token);

    Database* m_database = nullptr;

    QHttpServer* m_httpServer = nullptr;
    QTcpServer* m_tcpServer = nullptr;
};
