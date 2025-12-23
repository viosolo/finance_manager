#include "../headers/expense.h"


Expense::Expense(double amount, const QDate& date, ExpenseCategory category)
    : FinanceRecord(amount, date), category(category) {}
Expense::Expense()
    : FinanceRecord(0.0, QDate()), category(ExpenseCategory::Unknown) {}

QString Expense::categoryToString(ExpenseCategory category)
{
    switch(category) {
        case ExpenseCategory::Food: return "Food";
        case ExpenseCategory::Shopping: return "Shopping";
        case ExpenseCategory::OtherPayments: return "OtherPayments";
        case ExpenseCategory::Health: return "Health";
        case ExpenseCategory::Entertainment: return "Entertainment";
        case ExpenseCategory::Transport: return "Transport";
        default: return "Unknown";
    }
}

ExpenseCategory Expense::stringToCategory(const QString& str)
{
    if (str == "Food") return ExpenseCategory::Food;
    if (str == "Shopping") return ExpenseCategory::Shopping;
    if (str == "Other") return ExpenseCategory::OtherPayments;
    if (str == "Health") return ExpenseCategory::Health;
    if (str == "Entertainment") return ExpenseCategory::Entertainment;
    if (str == "Transport") return ExpenseCategory::Transport;
    return ExpenseCategory::Unknown;
}
QString Expense::printDisplay() const {
    return getDate().toString("dd.MM.yyyy") + " | " +
           QString::number(getAmount(), 'f', 2) + " BYN | " +
           "🏷️ " + categoryToString(category);
}
Expense::~Expense()
{


}

void Expense::writeToText(QTextStream& out) const {
    out << date.toString("dd.MM.yyyy") << " "
        << amount << " "
        << categoryToString(category) << " "
        << "ID:" << getId() << "\n";
}


ExpenseCategory Expense::getCategory() const {
    return category;
}

void Expense::readFromText(const QString& line) {
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

