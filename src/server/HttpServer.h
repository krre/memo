#pragma once
#include <QHttpServer>

class HttpServer : public QObject {
public:
    HttpServer(QObject* parent = nullptr);

    void start(quint16 port);
    void stop();

private:
    QHttpServer* m_httpServer = nullptr;
};
