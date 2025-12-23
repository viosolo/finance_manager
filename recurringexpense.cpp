#include "recurringexpense.h"

RecurringExpense::RecurringExpense(double amount, const QDate& date, ExpenseCategory category, int recurrenceDays)
    : Expense(amount, date, category), recurrenceDays(recurrenceDays) {}

RecurringExpense::RecurringExpense() : Expense(), recurrenceDays(0) {}

RecurringExpense::~RecurringExpense() {}

int RecurringExpense::getRecurrenceDays() const {
    return recurrenceDays;
}

QDate RecurringExpense::getNextDueDate() const {
    return date.addDays(recurrenceDays);
}

bool RecurringExpense::isDueOn(const QDate& targetDate) const {
    int daysSinceStart = date.daysTo(targetDate);
    return recurrenceDays > 0 && daysSinceStart >= 0 && daysSinceStart % recurrenceDays == 0;
}

void RecurringExpense::writeToText(QTextStream& out) const {
    out << date.toString("dd.MM.yyyy") << " "                  // дата
        << amount << " "                                       // сумма
        << categoryToString(category) << " "                   // категория
        << "ID:" << getId() << " "                             // ID
        << "Recurring " << recurrenceDays << "\n";             // флаг и период
}

void RecurringExpense::readFromText(const QString& line) {
    QStringList parts = line.split(' ');



    if (parts.size() >= 6) {
        date = QDate::fromString(parts[0], "dd.MM.yyyy");
        amount = parts[1].toDouble();
        category = stringToCategory(parts[2]);

        if (parts[3].startsWith("ID:")) {
            id = parts[3].mid(3);
        }

        if (parts[4] == "RECURRING") {
            recurrenceDays = parts[5].toInt();
        }


    }
}


QString RecurringExpense::printDisplay() const {
    return getDate().toString("dd.MM.yyyy") + " | " +
           QString::number(getAmount(), 'f', 2) + " BYN | " +
           "🏷️ " + categoryToString(getCategory()) + " | 🔁 every " +
           QString::number(recurrenceDays) + " days";
}
