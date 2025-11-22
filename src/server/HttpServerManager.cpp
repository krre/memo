#include "HttpServerManager.h"
#include "database/Database.h"
#include "core/SafeString.h"
#include "handler/NameHandler.h"
#include "handler/NotesHandler.h"
#include <QHttpServer>
#include <QSslServer>
#include <QFile>
#include <QSslKey>

HttpServerManager::HttpServerManager(Database* database, QObject* parent)
    : QObject(parent), m_database(database) {
}

void HttpServerManager::start(quint16 port, const SafeString& token, const SafeString& certificatePath, const SafeString& privateKeyPath) {
    stop();

    QFile certFile(certificatePath);

    if (!certFile.open(QIODevice::ReadOnly)) {
        qCritical().noquote() << "Failed to open certificate:" << certificatePath;
        return;
    }

    QSslCertificate certificate(&certFile, QSsl::Pem);
    certFile.close();

    QFile keyFile(privateKeyPath);

    if (!keyFile.open(QIODevice::ReadOnly)) {
        qCritical().noquote() << "Failed to open private key:" << privateKeyPath;
        return;
    }

    QSslKey privateKey(&keyFile, QSsl::Rsa, QSsl::Pem);
    keyFile.close();

    m_httpServer = new QHttpServer(this);
    m_tcpServer = new QSslServer;

    QSslConfiguration sslConfiguration;
    sslConfiguration.setLocalCertificate(certificate);
    sslConfiguration.setPrivateKey(privateKey);

    static_cast<QSslServer>(m_tcpServer).setSslConfiguration(sslConfiguration);

    startImpl(port, token);
}

void HttpServerManager::start(quint16 port, const SafeString& token) {
    stop();

    m_tcpServer = new QTcpServer(this);
    m_httpServer = new QHttpServer(this);

    startImpl(port, token);
}

void HttpServerManager::stop() {
    if (!m_httpServer) return;

    delete m_tcpServer;
    m_tcpServer = nullptr;

    delete m_httpServer;
    m_httpServer = nullptr;

    qInfo().noquote() << "Server stopped";
}

void HttpServerManager::startImpl(quint16 port, const SafeString& token) {
    if (!port) {
        qCritical().noquote() << "Server port is zero";
        return;
    }

    m_httpServer->route("/name", [=, this] (const QHttpServerRequest& request, QHttpServerResponder& responder) {
        responder.sendResponse(NameHandler(m_database).exec(request, token));
    });

    m_httpServer->route("/notes", [=, this] (const QHttpServerRequest& request, QHttpServerResponder& responder) {
        responder.sendResponse(NotesHandler(m_database).exec(request, token));
    });

    if (!m_tcpServer->listen(QHostAddress::Any, port) || !m_httpServer->bind(m_tcpServer)) {
        qCritical().noquote() << "Failed to start server on port" << port;
        stop();
    } else {
       qInfo().noquote() << "Server started on port" << port;
    }
}
