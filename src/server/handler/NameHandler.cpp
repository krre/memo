#include "NameHandler.h"
#include "database/Database.h"
#include <QHttpServerResponse>

NameHandler::NameHandler(Database* database) : Handler(database) {

}


QHttpServerResponse NameHandler::buildResponse() {
    return QHttpServerResponse(database()->name());
}
