#pragma once
#include "doublelinkedlist.h"
#include "financerecord.h"
#include <functional>
#include "expense.h"
#include "income.h"
#include <QDebug>
#include "recurringexpense.h"
class FinanceProcessor {
private:
    DoubleLinkedList<FinanceRecord*>* allRecords;
    double calculateRecurringAmountUpToToday(RecurringExpense* recurring, const QDate& today) ;

public:
    // Конструктор
    FinanceProcessor()=default;

double getTotalIncomeToday(const DoubleLinkedList<Income*>& incomes) ;
    void deleteExpense(DoubleLinkedList<FinanceRecord*>& list, FinanceRecord* value);
std::vector<Income*> getIncomesByDate(const DoubleLinkedList<Income*>& incomes, const QDate& date)  ;
std::vector<Expense*> getExpensesByDate(const DoubleLinkedList<Expense*>& expenses, const QDate& date);
bool isRecurringActiveOnDate(RecurringExpense* recurring, const QDate& targetDate) ;
    std::vector<Income*> getIncomesByCategory(DoubleLinkedList<Income*>& records, IncomeCategory category);
       std::vector<Expense*> getExpensesByCategory(DoubleLinkedList<Expense*>& records, ExpenseCategory category);


       template<typename RecordType>
       RecordType* deleteRecordById(DoubleLinkedList<RecordType*>& records, const QString& id) {
           int index = 0;
           typename DoubleLinkedList<RecordType*>::Iterator it = records.begin();
           while (it != records.end()) {
               RecordType* current = *it;
               if (current->getId() == id) {
                   records.erase(index);
                   return current;
               }
               ++it;
               ++index;
           }
           return nullptr;
       }

       template<typename T>
       std::vector<T> findRecordsByDateRange(
           DoubleLinkedList<T>& records,
           const QDate& startDate,
           const QDate& endDate)
       {
           std::vector<T> result;

           for (typename DoubleLinkedList<T>::Iterator it = records.begin(); it != records.end(); ++it) {
               T record = *it;
               if (record) {
                   QDate recordDate = record->getDate();
                   if (recordDate >= startDate && recordDate <= endDate) {
                       result.push_back(record);
                   }
               }
           }

           return result;
       }



       double getTotalExpensesToday(const DoubleLinkedList<Expense*>& expenses);

       template<typename T>
       bool addRecord(DoubleLinkedList<T*>& list, T* record) {
           if (!record) return false;

           qWarning() << "ДО добавления - Размер списка:" << list.size();


           list.push_back(record);

           qWarning() << "ПОСЛЕ добавления - Размер списка:" << list.size();

           return true;
       }
       template<typename T>
       T* findSmallestRecord(const DoubleLinkedList<T*>& records) {
           if (records.size() == 0) {
               return nullptr;
           }

           T* smallestRecord = nullptr;
           double minAmount = -1.0;

           for (int i = 0; i < records.size(); ++i) {
               T* record = records.get(i);
               if (!record) continue;

               if (minAmount == -1.0 || record->getAmount() < minAmount) {
                   minAmount = record->getAmount();
                   smallestRecord = record;
               }
           }

           return smallestRecord;
       }

       template<typename T>
       T* findLargestRecord(const DoubleLinkedList<T*>& records) {
           if (records.size() == 0) {
               return nullptr;
           }

           T* largestRecord = nullptr;
           double maxAmount = -1.0;

           for (int i = 0; i < records.size(); ++i) {
               T* record = records.get(i);
               if (!record) continue;

               if (record->getAmount() > maxAmount) {
                   maxAmount = record->getAmount();
                   largestRecord = record;
               }
           }

           return largestRecord;
       }
       template<typename T>
       void sortRecordsByDate(std::vector<T*>& records, bool ascending = true)
       {
           std::sort(records.begin(), records.end(),
               [ascending](T* a, T* b) {
                   if (!a || !b) return false;
                   if (ascending) {
                       return a->getDate() < b->getDate(); // старые сверху
                   } else {
                       return a->getDate() > b->getDate(); // новые сверху
                   }
               });
       }

       template<typename T>
       void sortRecordsBySum(std::vector<T*>& records, bool ascending = true) {
           std::sort(records.begin(), records.end(),
               [ascending](T* a, T* b) {
                   if (!a || !b) return false;
                   if (ascending) {
                       return a->getAmount() < b->getAmount();
                   } else {
                       return a->getAmount() > b->getAmount();
                   }
               });
       }

       template<typename T>
       double calculateTotalAmount(const std::vector<T*>& records,
                                                   const QDate& endDate)  {
           double totalAmount = 0.0;
           for (size_t i = 0; i < records.size(); ++i) {
               if (records[i] != nullptr) {
                   // Если это RecurringExpense, считаем по особой логике
                   if (auto recurring = dynamic_cast<RecurringExpense*>(records[i])) {
                       totalAmount +=this->calculateRecurringAmountUpToToday(recurring, endDate);
                   } else {
                       // Для обычных записей просто сумма
                       totalAmount += records[i]->getAmount();
                   }
               }
           }
           return totalAmount;
       }

       template<typename T>
       double getSum(const std::vector<T*>& records) {
           double total = 0.0;
           for (const auto& record : records) {
               total += record->getAmount();
           }
           return total;
       }
       };

