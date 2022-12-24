#include "HttpServer.h"

HttpServer::HttpServer(QObject* parent) : QObject(parent) {

}

void HttpServer::start(quint16 port) {
    m_httpServer = new QHttpServer(this);
    int result = m_httpServer->listen(QHostAddress::Any, port);

    if (result) {
        qInfo().noquote() << "Server started on port" << result;
    } else {
        qCritical().noquote() << "Failed to start server on port" << port;
    }
}

void HttpServer::stop() {
    m_httpServer = nullptr;
    qInfo().noquote() << "Server stopped";
}
