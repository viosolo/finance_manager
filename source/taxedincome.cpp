#include "../headers/taxedincome.h"
#include <QDebug>
#include <QRegularExpression>
#include <QRegularExpressionMatch>

TaxedIncome::TaxedIncome(double amount, const QDate& date, IncomeCategory category, double taxRate)
    : Income(amount, date, category), taxRate(taxRate) {}



double TaxedIncome::getTaxRate() const {
    return taxRate;
}
double TaxedIncome:: getAmountNotTax() const  {
       return amount;
   }

void TaxedIncome::writeToText(QTextStream& out) const {
    out << getDate().toString("dd.MM.yyyy") << " "             // [0] дата
        << getAmountNotTax() << " "                                  // [1] сумма
        << categoryToString(getCategory()) << " "        // [2] категория
        << "ID:" << getId() << " "                             // [3] ID
        << "Taxed " << getTaxRate() << "\n";
}


double TaxedIncome:: getAmount() const  {
       return amount * (1 - taxRate / 100.0);
   }


void TaxedIncome::readFromText(const QString& line) {
    QRegularExpression regex(
        "^"                                 // ^ - начало строки
        "(\\d{2}\\.\\d{2}\\.\\d{4})"        // 1. Дата (dd.MM.yyyy)
        "\\s+"                              // \\s+ - один или более пробельных символов
        "(\\S+)"                            // 2. Сумма (любые непробельные символы, например, "10")
        "\\s+"                              // Один или более пробелов
        "(\\S+)"                            // 3. Категория (любые непробельные символы, например, "Freelance")
        "\\s+"                              // Один или более пробелов
        "ID:"                               // Литерал "ID:"
        "([a-fA-F0-9-]+)"                   // 4. ID (UUID: шестнадцатеричные цифры и дефисы)
        "\\s+"                              // Один или более пробелов
        "Taxed"                             // Литерал "Taxed"
        "\\s+"                              // Один или более пробелов
        "([0-9.]+)"                         // 5. Налог (цифры и точка)
        "$"                                 // $ - конец строки
    );

    QRegularExpressionMatch match = regex.match(line);

    if (match.hasMatch()) {
        date = QDate::fromString(match.captured(1), "dd.MM.yyyy");
        amount = match.captured(2).toDouble();
        category = stringToCategory(match.captured(3));
        id = match.captured(4);
        taxRate = match.captured(5).toDouble();


        qDebug() << "  Tax Rate:" << taxRate;
    } else {
        qDebug() << "Failed to parse line (no match):" << line;

    }
}


QString TaxedIncome::printDisplay() const {
    return getDate().toString("dd.MM.yyyy") + " | " +
           QString::number(getAmount(), 'f', 2) + " BYN | " +
           "🏷️ " + categoryToString(getCategory()) + " | 💸 tax " +
           QString::number(taxRate) + "%";
}
