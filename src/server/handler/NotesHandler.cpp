#include "NotesHandler.h"
#include "database/Database.h"
#include <QHttpServerResponse>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

NotesHandler::NotesHandler(Database* database) : Handler(database) {

}

QHttpServerResponse NotesHandler::buildResponse() {
    QJsonArray result;

    for (const Note& note : database()->notes()) {
        QJsonObject obj;
        obj["id"] = note.id;
        obj["parentId"] = note.parentId;
        obj["pos"] = note.pos;
        obj["depth"] = note.depth;
        obj["title"] = note.title;
        obj["note"] = note.note;

        result.append(obj);
    }

    return QHttpServerResponse(result);
}
