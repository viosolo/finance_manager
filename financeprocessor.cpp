#include "financeprocessor.h"
#include <QDebug>

void FinanceProcessor::deleteExpense(DoubleLinkedList<FinanceRecord*>& list, FinanceRecord* value) {
    int index = 0;
    DoubleLinkedList<FinanceRecord*>::Iterator it = list.begin();
    DoubleLinkedList<FinanceRecord*>::Iterator endIt = list.end();

    while (it != endIt) {
        if (*it == value) {
            list.erase(index);
            break;
        }
        ++it;
        ++index;
    }
}

double FinanceProcessor::getTotalExpensesToday(const DoubleLinkedList<Expense*>& expenses) {
    double total = 0.0;
    QDate today = QDate::currentDate();

    for (int i = 0; i < expenses.size(); ++i) {
        Expense* expense = expenses.get(i);
        if (!expense) continue;

        if (RecurringExpense* recurring = dynamic_cast<RecurringExpense*>(expense)) {
            total += calculateRecurringAmountUpToToday(recurring, today);
        } else if (expense->getDate() <= today) {
            total += expense->getAmount();
        }
    }

    return total;
}


template
std::vector<Expense*> FinanceProcessor::findRecordsByDateRange<Expense*>(
    DoubleLinkedList<Expense*>& records,
    const QDate& startDate,
    const QDate& endDate);

std::vector<Expense*> FinanceProcessor::getExpensesByDate(const DoubleLinkedList<Expense*>& expenses, const QDate& date) {
    std::vector<Expense*> result;

    for (int i = 0; i < expenses.size(); ++i) {
        Expense* expense = expenses.get(i);
        if (!expense) continue;

        if (RecurringExpense* recurring = dynamic_cast<RecurringExpense*>(expense)) {
            if (isRecurringActiveOnDate(recurring, date)) {
                result.push_back(expense);
            }
        } else if (expense->getDate() == date) {
            result.push_back(expense);
        }
    }

    return result;
}



std::vector<Income*> FinanceProcessor::getIncomesByDate(const DoubleLinkedList<Income*>& incomes, const QDate& date) {
    std::vector<Income*> result;

    for (int i = 0; i < incomes.size(); ++i) {
        Income* income = incomes.get(i);
        if (!income) continue;

        if (income->getDate() == date) {
            result.push_back(income);
        }
    }

    return result;
}
double FinanceProcessor::getTotalIncomeToday(const DoubleLinkedList<Income*>& incomes) {
    double total = 0.0;
    QDate today = QDate::currentDate();

    for (int i = 0; i < incomes.size(); ++i) {
        Income* income = incomes.get(i);
        if (!income) continue;


        if (income->getDate() <= today) {
            total += income->getAmount();
        }
    }

    return total;
}
double FinanceProcessor::calculateRecurringAmountUpToToday(RecurringExpense* recurring, const QDate& today) {

       QDate currentDate = recurring->getDate();
        if (currentDate > today) return 0.0;
        double total = 0.0;
       int interval = recurring->getRecurrenceDays();



       while (currentDate <= today) {
           total += recurring->getAmount();
           currentDate = currentDate.addDays(interval);
       }

       return total;
   }

bool FinanceProcessor::isRecurringActiveOnDate(RecurringExpense* recurring, const QDate& targetDate) {
    QDate date = recurring->getDate();
    int interval = recurring->getRecurrenceDays();

    if (date > targetDate)
        return false;

    if (interval == 1)
        return true;

    QDate currentDate = date;
    while (currentDate <= targetDate) {
        if (currentDate == targetDate) {
            return true;
        }
        currentDate = currentDate.addDays(interval);
    }

    return false;
}
std::vector<Income*> FinanceProcessor::getIncomesByCategory(DoubleLinkedList<Income*>& records, IncomeCategory category) {
    std::vector<Income*> result;

    for (DoubleLinkedList<Income*>::Iterator it = records.begin(); it != records.end(); ++it) {
        Income* record = *it;
        if (record->getCategory() == category) {
            result.push_back(record);
        }
    }

    return result;
}

std::vector<Expense*> FinanceProcessor::getExpensesByCategory(DoubleLinkedList<Expense*>& records, ExpenseCategory category) {
    std::vector<Expense*> result;

    for (DoubleLinkedList<Expense*>::Iterator it = records.begin(); it != records.end(); ++it) {
        Expense* record = *it;
        if (record->getCategory() == category) {
            result.push_back(record);
        }
    }

    return result;
}




