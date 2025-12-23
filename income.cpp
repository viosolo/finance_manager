#include "income.h"


Income::Income(double amount, const QDate& date, IncomeCategory category)
    : FinanceRecord(amount, date), category(category) {}
Income::Income()
    : FinanceRecord(0.0, QDate()), category(IncomeCategory::Unknown) {}


Income::~Income()
{
}
QString Income::categoryToString(IncomeCategory category)
{
    switch(category) {
        case IncomeCategory::Salary: return "Salary";
        case IncomeCategory::Freelance: return "Freelance";
        case IncomeCategory::Rental: return "Rental";
        case IncomeCategory::Investment: return "Investment";
        case IncomeCategory::Gift: return "Gift";
        case IncomeCategory::Other: return "Other";
        default: return "Unknown";
    }
}

IncomeCategory Income::stringToCategory(const QString& str)
{
    if (str == "Salary") return IncomeCategory::Salary;
    if (str == "Freelance") return IncomeCategory::Freelance;
    if (str == "Rental") return IncomeCategory::Rental;
    if (str == "Investment") return IncomeCategory::Investment;
    if (str == "Gift") return IncomeCategory::Gift;
    if (str == "Other") return IncomeCategory::Other;
    return IncomeCategory::Unknown;
}

QString Income::printDisplay() const {
    return getDate().toString("dd.MM.yyyy") + " | " +
           QString::number(getAmount(), 'f', 2) + " BYN | " +
           "🏷️ " + categoryToString(category);
}
IncomeCategory Income:: getCategory() const {
        return category;
 }

void Income::writeToText(QTextStream& out) const {
    out << date.toString("dd.MM.yyyy") << " "
        << amount << " "
        << categoryToString(category) << " "
        << "ID:" << getId() << "\n";
}


 void Income::readFromText(const QString& line) {
     QStringList parts = line.split(' ');

     if (parts.size() >= 4) {
         date = QDate::fromString(parts[0], "dd.MM.yyyy");
         amount = parts[1].toDouble();
         category = stringToCategory(parts[2]);

         if (parts[3].startsWith("ID:")) {
             id = parts[3].mid(3);
         }
     }
 }
