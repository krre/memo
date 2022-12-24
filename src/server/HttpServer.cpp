#include "HttpServer.h"
#include "database/Database.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

HttpServer::HttpServer(Database* database, QObject* parent) : QObject(parent), m_database(database) {

}

void HttpServer::start(quint16 port) {
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

void HttpServer::stop() {
    m_httpServer = nullptr;
    qInfo().noquote() << "Server stopped";
}

QString HttpServer::handleName() const {
    return m_database->name();
}

QString HttpServer::handleNotes() const {
    QVector<Database::Note> notes = m_database->notes();
    QJsonArray result;

    for (const Database::Note& note : notes) {
        QJsonObject obj;
        obj["id"] = note.id;
        obj["parentId"] = note.parentId;
        obj["pos"] = note.pos;
        obj["depth"] = note.depth;
        obj["title"] = note.title;
        obj["note"] = note.note;

        result.append(obj);
    }

    return QString(QJsonDocument(result).toJson(QJsonDocument::Compact));
}
