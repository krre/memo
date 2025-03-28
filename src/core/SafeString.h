#pragma once
#include <QString>

class SafeString : public QString {
public:
    SafeString(const QString& other);
};
