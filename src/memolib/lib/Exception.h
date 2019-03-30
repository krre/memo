#pragma once
#include <stdexcept>
#include <QString>

namespace MemoLib {

class Exception : std::exception {

public:
    Exception() = default;
    virtual QString text() const = 0;
};

class RuntimeError : Exception {

public:
    RuntimeError(const QString& error);
    virtual QString text() const override;

private:
    QString error;
};

} // MemoLib
