#pragma once
#include "Handler.h"

class NameHandler : public Handler {
public:
    NameHandler(Database* database);

protected:
    QHttpServerResponse buildResponse() override;
};
