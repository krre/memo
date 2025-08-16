#include "NotesHandler.h"
#include "database/Database.h"
#include <QHttpServerResponse>
#include <QJsonObject>
#include <QJsonArray>

NotesHandler::NotesHandler(Database* database) : Handler(database) {

}

QHttpServerResponse NotesHandler::buildResponse() {
    QJsonArray result;
    const auto notes = database()->notes();

    for (const Note& note : notes) {
        QJsonObject obj;
        obj["id"] = note.id.toJson();
        obj["parentId"] = note.parentId.toJson();
        obj["pos"] = note.pos;
        obj["depth"] = note.depth;
        obj["title"] = note.title;
        obj["note"] = note.note;

        result.append(obj);
    }

    return QHttpServerResponse(result);
}
