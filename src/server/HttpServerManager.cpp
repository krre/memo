#include "HttpServerManager.h"
#include "database/Database.h"
#include "core/SolidString.h"
#include "handler/NameHandler.h"
#include "handler/NotesHandler.h"
#include <QHttpServer>
#include <QFile>
#include <QSslKey>

HttpServerManager::HttpServerManager(Database* database, QObject* parent)
    : QObject(parent), m_database(database) {
}

void HttpServerManager::start(quint16 port, const SolidString& token, const SolidString& certificatePath, const SolidString& privateKeyPath) {
    stop();

    QFile certFile(certificatePath);
    certFile.open(QIODevice::ReadOnly);
    QSslCertificate certificate(&certFile, QSsl::Pem);
    certFile.close();

    QFile keyFile(privateKeyPath);
    keyFile.open(QIODevice::ReadOnly);
    QSslKey privateKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    keyFile.close();

    m_httpServer = new QHttpServer(this);
    m_httpServer->sslSetup(certificate, privateKey);

    startImpl(port, token);
}

void HttpServerManager::start(quint16 port, const SolidString& token) {
    stop();
    m_httpServer = new QHttpServer(this);
    startImpl(port, token);
}

void HttpServerManager::stop() {
    if (!m_httpServer) return;

    delete m_httpServer;
    m_httpServer = nullptr;
    qInfo().noquote() << "Server stopped";
}

void HttpServerManager::startImpl(quint16 port, const SolidString& token) {
    if (!port) {
        qCritical().noquote() << "Server port is zero";
        return;
    }

    m_httpServer->route("/name", [=, this] (const QHttpServerRequest& request, QHttpServerResponder&& responder) {
        NameHandler handler(m_database);
        responder.sendResponse(handler.exec(request, token));
    });

    m_httpServer->route("/notes", [=, this] (const QHttpServerRequest& request, QHttpServerResponder&& responder) {
        NotesHandler handler(m_database);
        responder.sendResponse(handler.exec(request, token));
    });

    if (int result = m_httpServer->listen(QHostAddress::Any, port)) {
        qInfo().noquote() << "Server started on port" << result;
    } else {
        qCritical().noquote() << "Failed to start server on port" << port;
    }
}
