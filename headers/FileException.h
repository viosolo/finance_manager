#ifndef FILEEXCEPTION_H
#define FILEEXCEPTION_H
#include "finexception.h"

// Базовое исключение для всех файловых ошибок
class FileException : public FinException
{
public:
    explicit FileException(const QString& message = "File error")
        : FinException(message)
    {
    }

    FileException(const FileException& other)
        : FinException(other)
    {
    }

    virtual ~FileException() = default;
};
#endif // FILEEXCEPTION_H
