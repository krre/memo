#include "Handler.h"
#include <QHttpServerRequest>
#include <QHttpServerResponse>

Handler::Handler(Database* database) : m_database(database) {

}

QHttpServerResponse Handler::exec(const QHttpServerRequest& request, const QString& key) {
    bool accessOk = false;
    auto headers = request.headers();

    for (auto it = headers.cbegin(); it != headers.cend(); it++) {
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
