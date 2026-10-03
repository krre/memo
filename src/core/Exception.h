#pragma once
#include <QString>

class Exception : public std::exception {
public:
    Exception() = default;
    virtual QString error() const = 0;
    const char* what() const noexcept override;

private:
    mutable std::string m_errorString;
};

class RuntimeError : public Exception {
public:
    RuntimeError(const QString& error);
    QString error() const override;

private:
    QString m_error;
};
