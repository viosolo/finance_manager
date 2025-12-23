#ifndef INTERVALEXCEPTION_H
#define INTERVALEXCEPTION_H

#include "finexception.h"

class IntervalException : public FinException
{
public:
    IntervalException()
        : FinException("Interval cannot be 0!")
    {
    }

    IntervalException(const IntervalException& other)
        : FinException(other)
    {
    }

    virtual ~IntervalException() = default;
};

#endif // INTERVALEXCEPTION_H
