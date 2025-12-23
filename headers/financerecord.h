#ifndef FINANCERECORD_H
#define FINANCERECORD_H

#include <QString>
#include <QDate>
#include <QTextStream>
#include <QUuid>

class FinanceRecord
{
protected:
    double amount;
    QDate date;
    QString id;

public:
    FinanceRecord(double amount, const QDate& date);
     void setId(const QString& newId) ;
    virtual ~FinanceRecord();
void  generateId() ;

    virtual void writeToText(QTextStream& out) const;
    virtual QString printDisplay() const;

   virtual double getAmount() const;
    QDate getDate() const;
    QString getId() const;
};


template<typename T>
QTextStream& operator<<(QTextStream& out, const T& object) {
    object.writeToText(out);
    return out;
}

#endif // FINANCERECORD_H



