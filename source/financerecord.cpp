#include "../headers/financerecord.h"

FinanceRecord:: FinanceRecord(double amount, const QDate& date)
        : amount(amount), date(date) {}

FinanceRecord::~FinanceRecord() = default;


void FinanceRecord::writeToText(QTextStream& out) const {

    out << id << ","
        << QString::number(amount) << ","
        << date.toString("yyyy-MM-dd") << "\n";
}

QString FinanceRecord::printDisplay() const {
    return date.toString("dd.MM.yyyy") + " — " + QString::number(amount) + " BYN";
}


double FinanceRecord::getAmount() const {
    return amount;
}
void FinanceRecord:: generateId() {
       id = QUuid::createUuid().toString(QUuid::WithoutBraces);
   }
QDate FinanceRecord::getDate() const {
    return date;
}
  void FinanceRecord:: setId(const QString& newId) {
      id = newId;
  }
QString FinanceRecord::getId() const {
    return id;
}
