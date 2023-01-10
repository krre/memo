#pragma once

class QHttpServerRequest;
class QHttpServerResponse;
class QString;

class Database;

class Handler {
public:
    Handler(Database* database);
    QHttpServerResponse exec(const QHttpServerRequest& request, const QString& key);

protected:
    virtual QHttpServerResponse buildResponse() = 0;
    Database* database() const;

private:
    Database* m_database = nullptr;
};
