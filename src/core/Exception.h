#pragma once
#include <QString>

class Exception : std::exception {
public:
    Exception() = default;
    virtual QString error() const = 0;
    const char* what() const noexcept override;
};

class RuntimeError : public Exception {

public:
    RuntimeError(const QString& error);
    QString error() const override;

private:
    QString m_error;
};
