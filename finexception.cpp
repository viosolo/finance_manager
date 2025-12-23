#include "finexception.h"


QString FinException::getMessage() const
{
    return message;
}

void FinException::setMessage(const QString& otherMessage)
{
    message = otherMessage;
}

const char* FinException::what() const noexcept
{
    return message.toUtf8().constData();
}
