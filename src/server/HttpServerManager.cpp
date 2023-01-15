#include "HttpServerManager.h"
#include "database/Database.h"
#include "core/SolidString.h"
#include "handler/NameHandler.h"
#include "handler/NotesHandler.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QSslKey>

HttpServerManager::HttpServerManager(Database* database, QObject* parent) : QObject(parent), m_database(database) {

}

void HttpServerManager::start(quint16 port, const SolidString& token, const SolidString& certificatePath, const SolidString& privateKeyPath) {
    stop();

    if (!port) {
        qCritical().noquote() << "Server port is zero";
        return;
    }

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

    m_httpServer->route("/name", [=] (const QHttpServerRequest& request, QHttpServerResponder&& responder) {
        NameHandler handler(m_database);
        responder.sendResponse(handler.exec(request, token));
    });

    m_httpServer->route("/notes", [=] (const QHttpServerRequest& request, QHttpServerResponder&& responder) {
        NotesHandler handler(m_database);
        responder.sendResponse(handler.exec(request, token));
    });

    int result = m_httpServer->listen(QHostAddress::Any, port);

    if (result) {
        qInfo().noquote() << "Server started on port" << result;
    } else {
        qCritical().noquote() << "Failed to start server on port" << port;
    }
}

void HttpServerManager::stop() {
    if (!m_httpServer) return;

    delete m_httpServer;
    m_httpServer = nullptr;
    qInfo().noquote() << "Server stopped";
}
