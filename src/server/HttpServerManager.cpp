#include "HttpServerManager.h"
#include "database/Database.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

HttpServerManager::HttpServerManager(Database* database, QObject* parent) : QObject(parent), m_database(database) {

}

void HttpServerManager::start(quint16 port, const QString& key) {
    stop();
    m_httpServer = new QHttpServer(this);

    m_httpServer->route("/name", [=] () {
        return handleName();
    });

    m_httpServer->route("/notes", [=] () {
        return handleNotes();
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

QString HttpServerManager::handleName() const {
    return m_database->name();
}

QByteArray HttpServerManager::handleNotes() const {
    QVector<Database::Note> notes = m_database->notes();
    QJsonArray result;

    for (const Database::Note& note : notes) {
        QJsonObject obj;
        obj["id"] = note.id;
        obj["parentId"] = note.parentId;
        obj["pos"] = note.pos;
        obj["depth"] = note.depth;
        obj["title"] = QJsonValue::fromVariant(QByteArray(note.title.toUtf8()).toBase64());;
        obj["note"] = QJsonValue::fromVariant(QByteArray(note.note.toUtf8()).toBase64());

        result.append(obj);
    }

    return QJsonDocument(result).toJson(QJsonDocument::Compact);
}
