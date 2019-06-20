#pragma once
#include <stdexcept>
#include <QString>

namespace Common {

class Exception : std::exception {

public:
    Exception() = default;
    virtual QString error() const = 0;
    const char* what() const noexcept override;
};

class RuntimeError : Exception {

public:
    RuntimeError(const QString& error);
    QString error() const override;

private:
    QString m_error;
};

} // namespace Common
