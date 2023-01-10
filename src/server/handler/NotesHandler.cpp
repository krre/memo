#include "NotesHandler.h"
#include "database/Database.h"
#include <QHttpServerResponse>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

NotesHandler::NotesHandler(Database* database) : Handler(database) {

}

QHttpServerResponse NotesHandler::buildResponse() {
    QVector<Database::Note> notes = database()->notes();
    QJsonArray array;

    for (const Database::Note& note : notes) {
        QJsonObject obj;
        obj["id"] = note.id;
        obj["parentId"] = note.parentId;
        obj["pos"] = note.pos;
        obj["depth"] = note.depth;
        obj["title"] = QJsonValue::fromVariant(QByteArray(note.title.toUtf8()).toBase64());;
        obj["note"] = QJsonValue::fromVariant(QByteArray(note.note.toUtf8()).toBase64());

        array.append(obj);
    }

    return QHttpServerResponse(QJsonDocument(array).toJson(QJsonDocument::Compact));
}
