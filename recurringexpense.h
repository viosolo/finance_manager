#ifndef RECURRINGEXPENSE_H
#define RECURRINGEXPENSE_H

#include "expense.h"

class RecurringExpense : public Expense
{
private:
    int recurrenceDays;

public:
    RecurringExpense(double amount, const QDate& date, ExpenseCategory category, int recurrenceDays);
    RecurringExpense();
    ~RecurringExpense();

    int getRecurrenceDays() const;
    QDate getNextDueDate() const;
    bool isDueOn(const QDate& targetDate) const;

    void writeToText(QTextStream& out) const override;
    void readFromText(const QString& line) override;

    QString printDisplay() const override ;
};

#endif // RECURRINGEXPENSE_H
