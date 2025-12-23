#ifndef FINEXCEPTION_H
#define FINEXCEPTION_H

#include <QString>
#include <exception>

class FinException : public std::exception
{
protected:
    QString message;

public:
    explicit FinException(const QString& otherMessage = "unknown error")
        : message(otherMessage)
    {
    }

    FinException(const FinException& other)
        : message(other.message)
    {
    }

    virtual ~FinException() = default;

    QString getMessage() const ;

    void setMessage(const QString& otherMessage);

    const char* what() const noexcept override;


};

#endif // FINEXCEPTION_H
