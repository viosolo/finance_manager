#ifndef EXPENSE_H
#define EXPENSE_H
#include "financerecord.h"
#include <QTextStream>
#include <QDate>
#include <QString>
enum class ExpenseCategory {
    Unknown,
    Food,
    Shopping,
    OtherPayments,
    Health,
    Entertainment,
    Transport,
};

class Expense : public FinanceRecord
{
protected:
    ExpenseCategory category;

public:
    Expense(double amount, const QDate& date, ExpenseCategory category);
    Expense();
    virtual ~Expense();
    virtual void readFromText(const QString& line);


       static QString categoryToString(ExpenseCategory category);
       static ExpenseCategory stringToCategory(const QString& str);


    ExpenseCategory getCategory() const ;
 void writeToText(QTextStream& out) const override;
 QString printDisplay() const override ;
};

#endif // EXPENSE_H
