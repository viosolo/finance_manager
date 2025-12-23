#ifndef TAXEXCEPTION_H
#define TAXEXCEPTION_H

#include "finexception.h"

class TaxException : public FinException
{
public:
    TaxException()
        : FinException("Tax cannot be 0!")
    {
    }

    TaxException(const TaxException& other)
        : FinException(other)
    {
    }

    virtual ~TaxException() = default;
};

#endif // TAXEXCEPTION_H
