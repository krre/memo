#include "NameHandler.h"
#include "database/Database.h"
#include <QHttpServerResponse>
#include <QJsonObject>

NameHandler::NameHandler(Database* database) : Handler(database) {

}

QHttpServerResponse NameHandler::buildResponse() {
    QJsonObject obj;
    obj["name"] = database()->name();
    return QHttpServerResponse(obj);
}
