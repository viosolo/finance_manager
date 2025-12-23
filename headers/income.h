#ifndef INCOME_H
#define INCOME_H

#include "financerecord.h"
#include <QTextStream>
#include <QDate>
#include <QString>

enum class IncomeCategory {
    Unknown,
    Salary,
    Freelance,
    Rental,
    Investment,
    Gift,
    Other

};

class Income : public FinanceRecord
{
public:
    Income();
    Income(double amount, const QDate& date, IncomeCategory category);
    virtual ~Income();


   void writeToText(QTextStream& out) const override;
   virtual  void readFromText(const QString& line) ;

      static QString categoryToString(IncomeCategory category);
      static IncomeCategory stringToCategory(const QString& str);

    IncomeCategory getCategory() const;
    QString printDisplay() const  override;


protected:

    IncomeCategory category;
};

#endif // INCOME_H
