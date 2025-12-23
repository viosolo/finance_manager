#ifndef AMOUNTEXCEPTION_H
#define AMOUNTEXCEPTION_H

#include "finexception.h"

class AmountException : public FinException
{
public:
    AmountException()
        : FinException("Sum of expense cannot be 0!")
    {
    }

    AmountException(const AmountException& other)
        : FinException(other)
    {
    }

    virtual ~AmountException() = default;
};

#endif // AMOUNTEXCEPTION_H
