#include "Handler.h"
#include <QHttpServerRequest>
#include <QHttpServerResponse>

Handler::Handler(Database* database) : m_database(database) {

}

QHttpServerResponse Handler::exec(const QHttpServerRequest& request, const QString& token) {
    bool accessOk = false;
    auto headers = request.headers();

    for (auto& [ first, second ] : headers) {
        if (first.toLower() == "token" && second == token) {
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
