#ifndef TAXEDINCOME_H
#define TAXEDINCOME_H

#include "income.h"

class TaxedIncome : public Income {
private:
    double taxRate;

public:
    TaxedIncome(double amount, const QDate& date, IncomeCategory category, double taxRate);


    double getTaxRate() const;
   void readFromText(const QString& line) override ;
    void writeToText(QTextStream& out) const override;
double getAmount() const override;
    QString printDisplay() const override;
    double getAmountNotTax() const  ;
};

#endif // TAXEDINCOME_H
