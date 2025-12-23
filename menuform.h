#ifndef MENUFORM_H
#define MENUFORM_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>
#include <QList>
#include <QVector>
#include <QStack>
#include "expense.h"
#include "financemanager.h"
#include "financeprocessor.h"
#include "filemanager.h"
#include "doublelinkedlist.h"
#include "recurringexpense.h"
#include "taxedincome.h"
#include "AmountException.h"
#include "CategoryException.h"
#include "TaxException.h"
#include "IntervalException.h"

namespace Ui {
class MenuForm;
}

class MenuForm : public QDialog
{
    Q_OBJECT

public:
    explicit MenuForm(QWidget *parent = nullptr);
    ~MenuForm();

private slots:

    void onincomeComboBoxChanged(int index);
    void onCategoryComboBoxChanged(int index);
    void onDateSearchEnterPressed();

    void on_addExpenseButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_addButton_clicked();
void onNavigationButtonClicked();
    void on_pushButton_clicked();

void onRecurringCheckBoxToggled(bool checked);

void on_addIncomeClickButton_clicked();

void on_addIncomeButton_clicked();

void on_giftButton_clicked();

void on_pushButton_13_clicked();

void on_pushButton_15_clicked();

void on_pushButton_16_clicked();

void on_deleteExpenseButton_clicked();

void on_pushButton_12_clicked();

void on_pushButton_14_clicked();

void on_pushButton_9_clicked();

void on_undoExpenseButton_clicked();

void on_redoExpenseButton_clicked();

void on_unDelButton_clicked();

void on_pushButton_19_clicked();

void on_pushButton_18_clicked();

void on_pushButton_17_clicked();

void on_pushButton_20_clicked();

void on_pushButton_21_clicked();

void on_taxedCheckBox_stateChanged(int arg1);

void on_pushButton_8_clicked();

void on_deleteIncomeButton_clicked();

void on_pushButton_26_clicked();

void on_cancelIncomeButton_clicked();

void on_reDelButton_clicked();

void on_pushButton_27_clicked();

void on_pushButton_29_clicked();

void on_pushButton_28_clicked();

void on_pushButton_30_clicked();

void on_pushButton_31_clicked();

void on_pushButton_24_clicked();

void on_findButton_clicked();

void on_redoIncomeButton_2_clicked();

void on_pushButton_25_clicked();

void on_pushButton_32_clicked();

void on_pushButton_34_clicked();


void on_pushButton_38_clicked();

void on_cancelIncomeButton_2_clicked();

void on_pushButton_11_clicked();

void on_pushButton_10_clicked();


private:
    Ui::MenuForm *ui;

    FileManager* fileManager;
    FinanceProcessor* processor;

    DoubleLinkedList<Income*> incomes;
    DoubleLinkedList<Expense*> expenses;
    ExpenseCategory expenseCategory = ExpenseCategory::Unknown;
    IncomeCategory incomeCategory = IncomeCategory::Unknown;


  QMap<QPushButton*, QWidget*> navigationMap;
  QStack<Expense*> expenseUndoStack;
     QStack<Expense*> expenseRedoStack;

     QStack<Income*> incomeUndoStack;
     QStack<Income*> incomeRedoStack;

     QStack<Expense*> expenseDelUndoStack;
        QStack<Expense*> expenseDelRedoStack;
        QStack<Income*> incomeDelUndoStack;
        QStack<Income*> incomeDelRedoStack;

   void closeEvent(QCloseEvent* event) override;
public:

    void showMessage(const QString& title, const QString& message, const QString& type);
    template<typename T>
        void addToSpecificList(T* record, DoubleLinkedList<T*>& list);
void updateBalance() ;

void updateDeleteIncomeComboBox(const std::vector<Income*>& incomes);
    void updateIncomesForCategory(const std::vector<Income*>& incomes);

   void showExpensesByCategory(ExpenseCategory category);
   void showIncomesByCategory(IncomeCategory category) ;

   void toggleSideBar();
   void updateExpensesForCategory(const std::vector<Expense*>& expenses);



QDate parseDateFromString(const QString& dateText);


      void onIncomeComboBoxOpened();
void onCategoryComboBoxOpened();
 bool eventFilter(QObject *watched, QEvent *event) override;
 void restoreExpense(Expense* expense);
void setExpenseControls(bool visible) ;
 void setIncomeCategoryControlsVisible(bool visible) ;
void loadData();
void setConnections();
void setupSpinBoxes() ;
void showList(const std::vector<Expense*>& expenses,  const std::vector<Income*>& incomes);

template<typename T>
void showFoundRecord(T* record, const QString& title, const QString& description) {
    if (!record) {
        showMessage("Information", "No records found", "info");
        return;
    }

    QString emoji = "📋";
    if (description.contains("Largest", Qt::CaseInsensitive)) {
        emoji = "💥";
    } else if (description.contains("Smallest", Qt::CaseInsensitive)) {
        emoji = "🐭";
    }

    QString message = QString(
        "%1 %2\n"
        "%3"
    ).arg(emoji)
     .arg(description)
     .arg(record->printDisplay());

    showMessage(title, message, "info");
}


template<typename T>
void clearStack(QStack<T*>& stack)
{
    while (!stack.isEmpty()) {
        delete stack.pop();
    }
}



};



#endif // MENUFORM_H

