#pragma once
#include "Handler.h"

class NotesHandler : public Handler {
public:
    NotesHandler(Database* database);

protected:
    QHttpServerResponse buildResponse() override;
};
