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
        obj["title"] = note.title;
        obj["note"] = note.note;

        array.append(obj);
    }

    return QHttpServerResponse(array);
}
