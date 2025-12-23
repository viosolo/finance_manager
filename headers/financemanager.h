#ifndef FINANCEMANAGER_H
#define FINANCEMANAGER_H

#include "financerecord.h"
#include "expense.h"
#include "income.h"
class FinanceManager {
public:
    ~FinanceManager();
 void removeRecordByDate(std::vector<FinanceRecord*>& records, const std::string& targetDate);

std::vector<Expense*> getExpensesByCategory(ExpenseCategory category) const;
std::vector<Income*>getIncomeByCategory(IncomeCategory category) const;
          bool addExpense(Expense* expense);
          bool addIncome(Income* income);
 bool loadIncomeFromFile();
             bool loadExpensesFromFile();

private:
             std::vector<Expense*> allExpenses;
                  std::vector<Income*> allIncome;

};
#endif // FINANCEMANAGER_H
