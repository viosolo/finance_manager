#include "financemanager.h"

#include "recurringexpense.h"
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QString>


FinanceManager::~FinanceManager() {

}
std::vector<Expense*> FinanceManager::getExpensesByCategory(ExpenseCategory category) const
{
    std::vector<Expense*> result;
    for (Expense* expense : allExpenses) {
        if (expense->getCategory() == category) {
            result.push_back(expense);
        }
    }
    return result;
}
std::vector<Income*> FinanceManager::getIncomeByCategory(IncomeCategory category) const
{
    std::vector<Income*> result;
    for (Income* income : allIncome) {
        if (income->getCategory() == category) {
            result.push_back(income);
        }
    }
    return result;
}
void FinanceManager::removeRecordByDate(std::vector<FinanceRecord*>& records, const std::string& targetDate) {
    for (auto it = records.begin(); it != records.end(); ) {
        if ((*it)->getDate().toString().toStdString() == targetDate) {

            delete *it;
            it = records.erase(it);
        } else {
            ++it;
        }
    }
}




bool FinanceManager::addExpense(Expense* expense)
{
    if (!expense) return false;

  allExpenses.push_back(expense);

    QFile file("E:/budgetFinance/finance_budget/expenses.txt");
    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл для записи";
        return false;
    }

    QTextStream out(&file);
    expense->writeToText(out);
    return true;
}

bool FinanceManager::addIncome(Income* income)
{
    if (!income) return false;

  allIncome.push_back(income);

  QFile file("E:/budgetFinance/finance_budget/income.txt");

    if (!file.open(QIODevice::Append | QIODevice::Text)) {
        qWarning() << "Не удалось открыть файл для записи";
        return false;
    }

    QTextStream out(&file);
    income->writeToText(out);

    return true;
}

bool FinanceManager::loadExpensesFromFile()
{
//    QFile file("E:/budgetFinance/finance_budget/expenses.txt");

//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        qWarning() << "Не удалось открыть файл для чтения";
//        return false;
//    }

//    QTextStream in(&file);
//    while (!in.atEnd()) {
//        QString line = in.readLine();
//        if (line.trimmed().isEmpty()) continue;
//        Expense* expense=nullptr;
//         if (line.startsWith("RECURRING;")) {
//            expense = new RecurringExpense();
//        } else {
//            expense = new Expense();
//        }
//        expense->readFromText(line);

//      allExpenses.push_back(expense);
//    }

    return true;
}
bool FinanceManager::loadIncomeFromFile()
{
//    QFile file("E:/budgetFinance/finance_budget/income.txt");

//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        qWarning() << "Не удалось открыть файл для чтения";
//        return false;
//    }

//    QTextStream in(&file);
//    while (!in.atEnd()) {
//        QString line = in.readLine();
//        if (line.trimmed().isEmpty()) continue;
//Income* income= new Income();



//        income->readFromText(line);

//      allIncome.push_back(income);
//    }

    return true;
}
