#include "Handler.h"
#include <QHttpServerRequest>
#include <QHttpServerResponse>

Handler::Handler(Database* database) : m_database(database) {

}

QHttpServerResponse Handler::exec(const QHttpServerRequest& request, const QString& key) {
    bool accessOk = false;

    for (auto it = request.headers().cbegin(); it != request.headers().cend(); it++) {
        if (it->first.toLower() == "token" && it->second == key) {
            accessOk = true;
            break;
        }
    }

    if (!accessOk) {
        return QHttpServerResponse(QHttpServerResponder::StatusCode::Unauthorized);
    }

    return buildResponse();
}

Database* Handler::database() const {
    return m_database;
}
