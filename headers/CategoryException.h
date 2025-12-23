#ifndef CATEGORYEXCEPTION_H
#define CATEGORYEXCEPTION_H

#include "finexception.h"

class CategoryException : public FinException
{
public:
    CategoryException()
        : FinException("The category of expense wasn't chosen!")
    {
    }

    CategoryException(const CategoryException& other)
        : FinException(other)
    {
    }

    virtual ~CategoryException() = default;
};

#endif // CATEGORYEXCEPTION_H
