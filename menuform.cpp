#include "menuform.h"
#include "ui_menuform.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <QDebug>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>


MenuForm::MenuForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MenuForm)
{
    ui->setupUi(this);
    connect(ui->menuButton, &QPushButton::clicked, this, &MenuForm::toggleSideBar);
    ui->stackedWidget->setCurrentWidget(ui->homePage);

     ui->taxLabel->setVisible(false);
     ui->taxSpinBox->setVisible(false);
     ui->intervalSpinBox->setVisible(false);
     ui->intervalLabel->setVisible(false);
    ui->searchListWidget->setVisible(false);
    ui->widget_2->hide();
    setupSpinBoxes();

    fileManager = new FileManager(
        "E:/budgetFinance/finance_budget/income.txt",
        "E:/budgetFinance/finance_budget/expenses.txt"
    );
     processor = new FinanceProcessor();
    loadData();
    updateBalance();
    setConnections();
}

MenuForm::~MenuForm()
{
    delete ui;
    delete fileManager;
    delete processor;
    clearStack(expenseUndoStack);
    clearStack(expenseRedoStack);
    clearStack(incomeUndoStack);
    clearStack(incomeRedoStack);
    clearStack(expenseDelUndoStack);
    clearStack(expenseDelRedoStack);
    clearStack(incomeDelUndoStack);
    clearStack(incomeDelRedoStack);
}
void MenuForm::setConnections()
{
    connect(ui->dateSearchLineEdit, &QLineEdit::returnPressed,
            this, &MenuForm::onDateSearchEnterPressed);

    connect(ui->recurringCheckBox, &QCheckBox::toggled,
            this, &MenuForm::onRecurringCheckBoxToggled);

    connect(ui->incomeComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MenuForm::onincomeComboBoxChanged);

    connect(ui->categoryComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MenuForm::onCategoryComboBoxChanged);

    ui->categoryComboBox->installEventFilter(this);
    ui->incomeComboBox->installEventFilter(this);

    navigationMap[ui->homeButton] = ui->homePage;
    navigationMap[ui->analyticsButton] = ui->analyticsPage;
    navigationMap[ui->expenseButton] = ui->expensePage;
    navigationMap[ui->incomeButton] = ui->incomePage;
    navigationMap[ui->homeButton2] = ui->homePage;
    navigationMap[ui->analyticsButton2] = ui->analyticsPage;
    navigationMap[ui->expenseButton2] = ui->expensePage;
    navigationMap[ui->incomeButton2] = ui->incomePage;

    for (auto button : navigationMap.keys()) {
        connect(button, &QPushButton::clicked, this, &MenuForm::onNavigationButtonClicked);
    }
    if (ui->categoryComboBox->count() > 0) {
        onCategoryComboBoxChanged(ui->categoryComboBox->currentIndex());
    }
    if (ui->incomeComboBox->count() > 0) {
        onincomeComboBoxChanged(ui->incomeComboBox->currentIndex());
    }
}
void MenuForm::loadData()
{
    try {
        fileManager->loadIncomes(incomes);
    } catch (const FileException& e) {
        showMessage("Error", "Failed to load incomes: " + e.getMessage(), "error");
    }

    try {
        fileManager->loadExpenses(expenses);
    } catch (const FileException& e) {
        showMessage("Error", "Failed to load expenses: " + e.getMessage(), "error");
    }
    qDebug() << "Доходов загружено:" << incomes.size();
    qDebug() << "Расходов загружено:" << expenses.size();
}
void MenuForm::toggleSideBar()
{

    if (ui->widget->isVisible()) {
         ui->widget->hide();
         ui->widget_2->show();
     } else {
         ui->widget->show();
         ui->widget_2->hide();
     }
}
void MenuForm::setupSpinBoxes() {
    ui->doubleSpinBox->setDecimals(2);
    ui->doubleSpinBox->setMinimum(0.0);
    ui->doubleSpinBox->setMaximum(999999999.99);
    ui->doubleSpinBox->setSingleStep(1.0);

    ui->incomeSpinBox->setDecimals(2);
    ui->incomeSpinBox->setMinimum(0.0);
    ui->incomeSpinBox->setMaximum(999999999.99);
    ui->incomeSpinBox->setSingleStep(1.0);

    ui->dateEdit->setDate(QDate::currentDate());
    ui->dateEdit->setMaximumDate(QDate::currentDate());
    ui->dateEdit->setCalendarPopup(true);


    ui->incomeDateEdit->setDate(QDate::currentDate());
    ui->incomeDateEdit->setMaximumDate(QDate::currentDate());
    ui->incomeDateEdit->setCalendarPopup(true);

    ui->taxSpinBox->setDecimals(1);
      ui->taxSpinBox->setMinimum(0.0);
      ui->taxSpinBox->setMaximum(99.9);
      ui->taxSpinBox->setSingleStep(0.1);
      ui->taxSpinBox->setValue(0.0);

    ui->intervalSpinBox->setMinimum(1);
    ui->intervalSpinBox->setMaximum(365);
    ui->intervalSpinBox->setSingleStep(1);
    ui->intervalSpinBox->setValue(1);
}
void MenuForm::onRecurringCheckBoxToggled(bool checked)
{
    ui->intervalSpinBox->setVisible(checked);
    ui->intervalLabel->setVisible(checked);
}


void MenuForm::onincomeComboBoxChanged(int index)
{
    qDebug() << "=== onincomeComboBoxChanged ===";
    qDebug() << "Index:" << index;

    QString selectedText = ui->incomeComboBox->itemText(index);
    qDebug() << "Selected text:" << selectedText;

    incomeCategory = Income::stringToCategory(selectedText);
    qDebug() << "incomeCategory установлен:" << (int)incomeCategory;

    ui->categoryIncomeLabel->setText("Category was chosen: " + selectedText);
}
bool MenuForm::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::MouseButtonPress)
    {

        if (watched == ui->categoryComboBox)
        {
            qDebug() << "Category ComboBox был нажат (открывается выпадающий список)";

            onCategoryComboBoxOpened();
        }
        else if (watched == ui->incomeComboBox)
        {
            qDebug() << "Income ComboBox был нажат (открывается выпадающий список)";
            onIncomeComboBoxOpened();
        }
    }

    return QWidget::eventFilter(watched, event);
}
void MenuForm::onCategoryComboBoxOpened()
{
    qDebug() << "Слот onCategoryComboBoxOpened вызван. Принудительно обновляем метку.";

    if (ui->categoryComboBox->count() > 0) {
        onCategoryComboBoxChanged(ui->categoryComboBox->currentIndex());
    }
}

void MenuForm::onIncomeComboBoxOpened()
{
    qDebug() << "Слот onIncomeComboBoxOpened вызван. Принудительно обновляем метку.";
    if (ui->incomeComboBox->count() > 0) {
        onincomeComboBoxChanged(ui->incomeComboBox->currentIndex());
    }
}

void MenuForm::onCategoryComboBoxChanged(int index)
{
    qDebug() << "=== onCategoryComboBoxChanged ===";
    qDebug() << "Index:" << index;

    QString selectedText = ui->categoryComboBox->itemText(index);
    qDebug() << "Selected text:" << selectedText;

    expenseCategory = Expense::stringToCategory(selectedText);
    qDebug() << "expenseCategory установлен:" << (int)expenseCategory;

    ui->categoryLabel->setText("Category was chosen: " + Expense::categoryToString(expenseCategory));
}
void MenuForm::updateBalance() {
    double totalExToday = processor->getTotalExpensesToday(expenses);
    double totalInToday = processor->getTotalIncomeToday(incomes);
    double balanceToday = totalInToday - totalExToday;

    ui->label_38->setText(QString::number(totalExToday, 'f', 2));
    ui->label_34->setText(QString::number(totalInToday, 'f', 2));
    ui->label_balance->setText(QString::number(balanceToday, 'f', 2));



}
void MenuForm::onNavigationButtonClicked()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());
    if (!clickedButton) return;

    QWidget* targetPage = navigationMap.value(clickedButton, nullptr);
    if (targetPage) {
        ui->stackedWidget->setCurrentWidget(targetPage);
    }
}



template<typename T>
void MenuForm::addToSpecificList(T* record, DoubleLinkedList<T*>& list) {
    if (!record) return;

       if (!processor->addRecord(list, record)) {
           delete record;
           qWarning() << "Ошибка при добавлении записи";
           return;
       }

}
void MenuForm::showMessage(const QString& title, const QString& message, const QString& type)
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(title);

    QString formattedMessage = QString(
        "<div style='text-align: center; padding: 20px;'>"
        "<p style='font-size: 16px; font-weight: bold; margin: 0; color: white;'>%1</p>"
        "</div>"
    ).arg(message);

    msgBox.setText(formattedMessage);


    msgBox.setStyleSheet(R"(
        QMessageBox {
            background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1,
                                      stop: 0 #667eea, stop: 1 #764ba2);
            color: white;
            font-family: 'Segoe UI', Arial, sans-serif;
            font-size: 14px;
            border: none;
            border-radius: 15px;
            padding: 0px;
        }
        QMessageBox QLabel {
            color: white;
            font-size: 14px;
            background: transparent;
        }
        QMessageBox QPushButton {
            background: rgba(255,255,255,0.9);
            color: #333;
            border: none;
            padding: 10px 25px;
            border-radius: 8px;
            font-weight: bold;
            font-size: 13px;
            min-width: 80px;
            margin: 5px;
        }
        QMessageBox QPushButton:hover {
            background: rgba(255,255,255,1);
            border: 2px solid rgba(255,255,255,0.6);
        }
        QMessageBox QPushButton:pressed {
            background: rgba(255,255,255,0.8);
        }
    )");

    if (type == "success") {
        msgBox.setIcon(QMessageBox::Information);
    } else if (type == "error") {
        msgBox.setIcon(QMessageBox::Critical);
    } else if (type == "warning") {
        msgBox.setIcon(QMessageBox::Warning);
    } else {
        msgBox.setIcon(QMessageBox::Information);
    }

    msgBox.exec();
}
void MenuForm::on_addExpenseButton_clicked()
{
    try {
        double amount = ui->doubleSpinBox->value();
        if (amount == 0.0) {
            throw AmountException();
        }

        QDate date = ui->dateEdit->date();

        if (expenseCategory == ExpenseCategory::Unknown) {
            throw CategoryException();
        }

        Expense* expense = nullptr;

        if (ui->recurringCheckBox->isChecked()) {
            int interval = ui->intervalSpinBox->value();
            if (interval == 0) {
                throw IntervalException();
            }
            expense = new RecurringExpense(amount, date, expenseCategory, interval);
        } else {
            expense = new Expense(amount, date, expenseCategory);
        }

        expense->generateId();
        addToSpecificList(expense, expenses);
        showMessage("Success", "The expense was added!", "success");

        ui->categoryLabel->setText("✅ Category selected:");
        ui->intervalLabel->setVisible(false);
        ui->intervalSpinBox->setVisible(false);
        ui->recurringCheckBox->setChecked(false);
        expenseUndoStack.push(expense);
        expenseRedoStack.clear();
        ui->doubleSpinBox->setValue(0.0);
        ui->dateEdit->setDate(QDate::currentDate());
        expenseCategory = ExpenseCategory::Unknown;
        ui->categoryComboBox->setCurrentText("Food");

        updateBalance();
    }
    catch (const AmountException& e) {
        showMessage("Error", "Sum of expense cannot be 0!", "error");
        expenseCategory = ExpenseCategory::Unknown;
        return;
    }
    catch (const CategoryException& e) {
        showMessage("Error", "The category of expense wasn't chosen!", "error");
        return;
    }
    catch (const IntervalException& e) {
        showMessage("Error", "Interval cannot be 0!", "error");
        expenseCategory = ExpenseCategory::Unknown;
        return;
    }
}
void MenuForm::onDateSearchEnterPressed()
{

    QString dateText = ui->dateSearchLineEdit->text().trimmed();

    QDate selectedDate = parseDateFromString(dateText);

    if (!selectedDate.isValid()) {
        showMessage("Error", "❌ Invalid format of date!\nUse: DD.MM.YYYY", "error");
        return;
    }

    std::vector<Expense*> dateExpenses = processor->getExpensesByDate(expenses, selectedDate);
    std::vector<Income*> dateIncomes = processor->getIncomesByDate(incomes, selectedDate);

    ui->searchListWidget->clear();

    if (dateExpenses.empty() && dateIncomes.empty()) {
        ui->searchListWidget->setVisible(false);
        showMessage("Information", "📅 No records found for the selected date", "info");
    }
    else {
        ui->searchListWidget->setVisible(true);

        for (Expense* expense : dateExpenses) {
            QString itemText = QString("🔴 %1").arg(expense->printDisplay());
            QListWidgetItem* item = new QListWidgetItem(itemText);
            item->setData(Qt::UserRole, expense->getId());
            item->setData(Qt::UserRole + 1, "expense");
            item->setForeground(QColor(231, 76, 60)); // красный
            ui->searchListWidget->addItem(item);
        }

        for (Income* income : dateIncomes) {
            QString itemText = QString("🟢 %1").arg(income->printDisplay());
            QListWidgetItem* item = new QListWidgetItem(itemText);
            item->setData(Qt::UserRole, income->getId());
            item->setData(Qt::UserRole + 1, "income");
            item->setForeground(QColor(39, 174, 96)); // зеленый
            ui->searchListWidget->addItem(item);
        }

        showMessage("Date Search Results",
            QString("📅 Date: %1\n"
                    "🔴 Expenses: %2 records\n"
                    "🟢 Incomes: %3 records\n"
                    "📊 Total records: %4")
                .arg(selectedDate.toString("dd.MM.yyyy"))
                .arg(dateExpenses.size())
                .arg(dateIncomes.size())
                .arg(dateExpenses.size() + dateIncomes.size()),
            "info");
    }


    ui->dateSearchLineEdit->clear();

}


QDate MenuForm::parseDateFromString(const QString& dateText)
{
    QDate date = QDate::fromString(dateText, "dd.MM.yyyy");
    if (!date.isValid()) date = QDate::fromString(dateText, "d.M.yyyy");
    if (!date.isValid()) date = QDate::fromString(dateText, "dd.MM.yy");
    if (!date.isValid()) date = QDate::fromString(dateText, "d.M.yy");
    return date;
}



void MenuForm::updateExpensesForCategory(const std::vector<Expense*>& expenses)
{
 ui->deleteExpenseComboBox->clear();
   ui->expenseListWidget->clear();

      for (Expense* expense : expenses) {
        QString itemText = expense->printDisplay();

          QListWidgetItem* item = new QListWidgetItem(itemText);
          item->setData(Qt::UserRole, expense->getId());
          ui->expenseListWidget->addItem(item);
           ui->deleteExpenseComboBox->addItem(itemText,expense->getId());
      }


}

void MenuForm::setExpenseControls(bool visible) {
      ui->pushButton_16->setVisible(visible);
      ui->expenseListWidget->setVisible(visible);
      ui->pushButton_22->setVisible(!visible);
      ui->pushButton_28->setVisible(visible);
      ui->pushButton_29->setVisible(visible);
      ui->sumExLabel->setVisible(visible);
  }

void MenuForm::showExpensesByCategory(ExpenseCategory category) {
    auto filtered = processor->getExpensesByCategory(expenses, category);
    updateExpensesForCategory(filtered);

    ui->stackedWidget->setCurrentWidget(ui->viewPage);

    if (filtered.empty()) {
        setExpenseControls(false);
        ui->label_6->setText("No still expenses for this category");

    } else {
        setExpenseControls(true);

        double sum = processor->getSum(filtered);
        QString formattedSum = QLocale(QLocale::English).toString(sum, 'f', 0);
        QString labelText = QString("✦ 💰 TOTAL SUM: %1 ✦").arg(formattedSum);
        ui->sumExLabel->setText(labelText);
        ui->label_6->setText("Expenses from category: " + Expense::categoryToString(category));
    }
}


void MenuForm::on_pushButton_2_clicked()
{
      expenseCategory = ExpenseCategory::Food;
    showExpensesByCategory(ExpenseCategory::Food);

}

void MenuForm::on_pushButton_3_clicked()
{
     expenseCategory = ExpenseCategory::Health;
   showExpensesByCategory(ExpenseCategory::Health);

}

void MenuForm::on_pushButton_4_clicked()
{
     expenseCategory = ExpenseCategory::Shopping;
   showExpensesByCategory(ExpenseCategory::Shopping);

}

void MenuForm::on_pushButton_5_clicked()
{
     expenseCategory = ExpenseCategory::Transport;
    showExpensesByCategory(ExpenseCategory::Transport);

}

void MenuForm::on_pushButton_6_clicked()
{
     expenseCategory = ExpenseCategory::Entertainment;
   showExpensesByCategory(ExpenseCategory::Entertainment);
}

void MenuForm::on_pushButton_7_clicked()
{
     expenseCategory = ExpenseCategory::OtherPayments;
   showExpensesByCategory(ExpenseCategory:: OtherPayments);

}

void MenuForm::on_addButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->addExpensePage);
}

void MenuForm::on_pushButton_clicked()
{
     expenseCategory = ExpenseCategory::Unknown;
    ui->stackedWidget->setCurrentWidget(ui->expensePage);
}


void MenuForm::on_addIncomeClickButton_clicked()
{
    try {
         double amount = ui->incomeSpinBox->value();
         if (amount == 0.0) {
             throw AmountException();
         }
         QDate date = ui->incomeDateEdit->date();
         if (incomeCategory == IncomeCategory::Unknown) {
             throw CategoryException();
         }
         Income* income = nullptr;
         if (ui->taxedCheckBox->isChecked()) {
             double taxRate = ui->taxSpinBox->value();
             if (taxRate == 0) {
                 throw TaxException();
             }
             income = new TaxedIncome(amount, date, incomeCategory, taxRate);
         } else {
             income = new Income(amount, date, incomeCategory);
         }
         income->generateId();
         addToSpecificList(income, incomes);

         incomeUndoStack.push(income);
         incomeRedoStack.clear();


         showMessage("Success", "Income was added!", "success");
         ui->categoryIncomeLabel->setText("✅ Category selected:");
         ui->taxLabel->setVisible(false);
         ui->taxSpinBox->setVisible(false);
         ui->taxedCheckBox->setChecked(false);

         ui->incomeSpinBox->setValue(0.0);
         ui->incomeDateEdit->setDate(QDate::currentDate());
         ui->taxSpinBox->setValue(0.0);

         updateBalance();
     }
    catch (const AmountException& e) {
        showMessage("Error", "Sum of income cannot be 0!", "error");
        incomeCategory = IncomeCategory::Unknown;
        return;
        }
        catch (const CategoryException& e) {
        showMessage("Error", "The category of income wasn't chosen!", "error");
        return;
        }
        catch (const TaxException& e) {
        showMessage("Error", "Tax rate cannot be 0!", "error");
        incomeCategory = IncomeCategory::Unknown;
        return;
        }
}

void MenuForm::updateDeleteIncomeComboBox(const std::vector<Income*>& incomes)
{
    ui->deleteIncomeComboBox->clear();

    for (Income* income : incomes) {
        QString itemText = income->printDisplay();
        ui->deleteIncomeComboBox->addItem(itemText, income->getId());
    }
}

void MenuForm::updateIncomesForCategory(const std::vector<Income*>& incomes)
{
    ui->incomeListWidget->clear();

    for (Income* income : incomes) {
       QString itemText = income->printDisplay();
        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, income->getId());
        ui->incomeListWidget->addItem(item);

    }
     updateDeleteIncomeComboBox(incomes);
}

void MenuForm::on_addIncomeButton_clicked()
{
   ui->stackedWidget->setCurrentWidget(ui->addIncomePage);
}
void MenuForm::setIncomeCategoryControlsVisible(bool visible) {
     ui->pushButton_23->setVisible(!visible);
       ui->incomeListWidget->setVisible(visible);
       ui->pushButton_8->setVisible(visible);
       ui->pushButton_30->setVisible(visible);
       ui->pushButton_31->setVisible(visible);
       ui->sumInLabel->setVisible(visible);
   }
void MenuForm::showIncomesByCategory(IncomeCategory category) {
    auto filtered = processor->getIncomesByCategory(incomes, category);
    updateIncomesForCategory(filtered);
    ui->stackedWidget->setCurrentWidget(ui->incomeViewPage);

    if (filtered.empty()) {
        setIncomeCategoryControlsVisible(false);
        ui->label_7->setText("No still incomes for this category");

    } else {
        setIncomeCategoryControlsVisible(true);

        double sum = processor->getSum(filtered);
        QString formattedSum = QLocale(QLocale::English).toString(sum, 'f', 0);
        QString labelText = QString("✦ 💰 TOTAL SUM: %1 ✦").arg(formattedSum);
        ui->sumInLabel->setText(labelText);
        ui->label_7->setText("Incomes from category: " + Income::categoryToString(category));
    }
}

void MenuForm::closeEvent(QCloseEvent* event)
{
    qDebug() << "=== СОХРАНЕНИЕ ПРИ ЗАКРЫТИИ ===";
    qDebug() << "Количество расходов для сохранения:" << expenses.size();
    qDebug() << "Количество доходов для сохранения:" << incomes.size();
    for (int i = 0; i < expenses.size(); ++i) {
        Expense* exp = expenses.get(i);
        qDebug() << "Расход" << i << ":"
                 << "ID:" << exp->getId()
                 << "Категория:" << Expense::categoryToString(exp->getCategory())
                 << "Сумма:" << exp->getAmount()
                 << "Дата:" << exp->getDate().toString("dd.MM.yyyy");
    }

    try {
        fileManager->saveToFile<Expense>(expenses);
        qDebug() << "✓ Expenses saved";
    } catch (const FileException& e) {
        showMessage("Error", e.getMessage(), "error");
    }

    // Сохранение доходов
    try {
        fileManager->saveToFile<Income>(incomes);
        qDebug() << "✓ Incomes saved";
    } catch (const FileException& e) {
        showMessage("Error", e.getMessage(), "error");
    }

    event->accept();
}


void MenuForm::on_giftButton_clicked()
{
    showIncomesByCategory(IncomeCategory::Gift);
}

void MenuForm::on_pushButton_13_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->viewPage);
}

void MenuForm::on_pushButton_15_clicked()
{
   ui->stackedWidget->setCurrentWidget(ui->incomeViewPage);
}

void MenuForm::on_pushButton_16_clicked()
{
     ui->stackedWidget->setCurrentWidget(ui->deleteExpensePage);
}



void MenuForm::on_deleteExpenseButton_clicked()
{
    int index = ui->deleteExpenseComboBox->currentIndex();
       if (index < 0) return;

       QString selectedId = ui->deleteExpenseComboBox->itemData(index).toString();
       Expense* deleted = processor->deleteRecordById(expenses, selectedId);
       if (!deleted) return;
       ui->deleteExpenseComboBox->removeItem(index);

       int listIndex = -1;
       for (int i = 0; i < ui->expenseListWidget->count(); ++i) {
           QListWidgetItem* item = ui->expenseListWidget->item(i);
           QString itemId = item->data(Qt::UserRole).toString();
           if (itemId == selectedId) {
               listIndex = i;
               delete ui->expenseListWidget->takeItem(i);
               break;
           }
       }

       expenseDelUndoStack.push(deleted);
       expenseDelRedoStack.clear(); // очищаем только redo стек
    showMessage("Success", "Expense has been successfully deleted", "success");
      updateBalance();
}


void MenuForm::on_pushButton_12_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->expensePage);
}


void MenuForm::on_pushButton_14_clicked()
{
    ui->stackedWidget->setCurrentWidget(ui->incomePage);
}

void MenuForm::on_pushButton_9_clicked()
{
      incomeCategory = IncomeCategory::Unknown;
    ui->stackedWidget->setCurrentWidget(ui->incomePage);
}

void MenuForm::on_undoExpenseButton_clicked()
{
    if (expenseUndoStack.isEmpty()){

       showMessage("Information", "⚠️ Nothing to undo for expenses", "info");
        return;
    }

    Expense* lastAddedExpense = expenseUndoStack.pop();

    QString targetId = lastAddedExpense->getId();
     Expense* deletedExpense = processor->deleteRecordById<Expense>(expenses, targetId);
     if (!deletedExpense) {
            expenseUndoStack.push(lastAddedExpense);
            return;
        }
//    for (int i = 0; i < expenses.size(); ++i) {
//        if (expenses.get(i)->getId() == targetId) {
//            expenses.erase(i);
//            break;
//        }
  //  }
    for (int i = 0; i < ui->expenseListWidget->count(); ++i) {
        QListWidgetItem* item = ui->expenseListWidget->item(i);
        if (item->data(Qt::UserRole).toString() == targetId) {
            delete ui->expenseListWidget->takeItem(i);
            break;
        }
    }

    for (int i = 0; i < ui->deleteExpenseComboBox->count(); ++i) {
        if (ui->deleteExpenseComboBox->itemData(i).toString() == targetId) {
            ui->deleteExpenseComboBox->removeItem(i);
            break;
        }
    }

    expenseRedoStack.push(lastAddedExpense);

        showMessage("Success", "Recently added expense has been  deleted", "success");
    updateBalance();
}
void MenuForm::on_redoExpenseButton_clicked()
{
    if (expenseRedoStack.isEmpty()) {
        showMessage("Information", "⚠️ Nothing to redo for expenses", "info");
        return;
     }
       Expense* lastUndoneExpense = expenseRedoStack.pop();


       processor->addRecord(expenses, lastUndoneExpense);


       QString itemText = lastUndoneExpense->printDisplay();

       // ListWidget
       QListWidgetItem* item = new QListWidgetItem(itemText);
       item->setData(Qt::UserRole, lastUndoneExpense->getId());
       ui->expenseListWidget->addItem(item);

       // ComboBox
       ui->deleteExpenseComboBox->addItem(itemText, lastUndoneExpense->getId());

       expenseUndoStack.push(lastUndoneExpense);

       showMessage("Success", "Recently deleted expense has been  renewed", "success");
       updateBalance();
}


void MenuForm::on_unDelButton_clicked()
{
    if (expenseDelUndoStack.isEmpty()) {
        showMessage("Information", "⚠️ No expense deletions to undo", "info");
        return;
    }

    Expense* expense = expenseDelUndoStack.pop();
    restoreExpense(expense);
    expenseDelRedoStack.push(expense);

    showMessage("Success", "Recently deleted expense has been  renewed", "success");
    updateBalance();
}

void MenuForm::restoreExpense(Expense* expense)
{

    processor->addRecord(expenses, expense);


    QString displayText = expense->printDisplay();
    ui->deleteExpenseComboBox->addItem(displayText, expense->getId());


    QListWidgetItem* item = new QListWidgetItem(displayText);
    item->setData(Qt::UserRole, expense->getId());
    ui->expenseListWidget->addItem(item);
}
void MenuForm::on_pushButton_19_clicked()
{
      incomeCategory = IncomeCategory::Rental;
    showIncomesByCategory(IncomeCategory::Rental);
}

void MenuForm::on_pushButton_18_clicked()
{
      incomeCategory = IncomeCategory::Investment;
    showIncomesByCategory(IncomeCategory::Investment);
}

void MenuForm::on_pushButton_17_clicked()
{
      incomeCategory = IncomeCategory::Freelance;
    showIncomesByCategory(IncomeCategory::Freelance);
}

void MenuForm::on_pushButton_20_clicked()
{
      incomeCategory = IncomeCategory::Salary;
    showIncomesByCategory(IncomeCategory::Salary);
}

void MenuForm::on_pushButton_21_clicked()
{
     incomeCategory = IncomeCategory::Other;
    showIncomesByCategory(IncomeCategory::Other);
}

void MenuForm::on_taxedCheckBox_stateChanged(int arg1)
{
    bool checked = (arg1 == Qt::Checked);
    ui->taxLabel->setVisible(checked);
    ui->taxSpinBox->setVisible(checked);
}

void MenuForm::on_pushButton_8_clicked()
{
      ui->stackedWidget->setCurrentWidget(ui->deleteIncomePage);
}


void MenuForm::on_deleteIncomeButton_clicked()
{
    int index = ui->deleteIncomeComboBox->currentIndex();
    if (index < 0) return;

    QString selectedId = ui->deleteIncomeComboBox->itemData(index).toString();
    Income* deleted = processor->deleteRecordById(incomes, selectedId);
    if (!deleted) return;


    ui->deleteIncomeComboBox->removeItem(index);

    // Удаляем из списка
    int listIndex = -1;
    for (int i = 0; i < ui->incomeListWidget->count(); ++i) {
        QListWidgetItem* item = ui->incomeListWidget->item(i);
        QString itemId = item->data(Qt::UserRole).toString();
        if (itemId == selectedId) {
            listIndex = i;
            delete ui->incomeListWidget->takeItem(i);
            break;
        }
    }
    incomeDelUndoStack.push(deleted);
    incomeDelRedoStack.clear();

 showMessage("Success", "Income has been deleted", "success");
     updateBalance();
}


void MenuForm::on_pushButton_26_clicked()
{

}

void MenuForm::on_cancelIncomeButton_clicked()
{

       if (incomeUndoStack.isEmpty()) {
           qDebug() << "Undo stack is empty";
           showMessage("Info", "No recently added income to cancel", "info");
           return;
       }


       Income* lastAddedIncome = incomeUndoStack.pop();
       QString targetId = lastAddedIncome->getId();

       qDebug() << "Undoing addition of income with ID:" << targetId;
       Income* deletedIncome = processor->deleteRecordById<Income>(incomes, targetId);
        if (!deletedIncome) {
            incomeUndoStack.push(lastAddedIncome);
            qDebug() << "Income not found in list, restored to undo stack";
            return;
        }



       for (int i = 0; i < ui->incomeListWidget->count(); ++i) {
           QListWidgetItem* item = ui->incomeListWidget->item(i);
           if (item->data(Qt::UserRole).toString() == targetId) {
               delete ui->incomeListWidget->takeItem(i);
               break;
           }
       }


       for (int i = 0; i < ui->deleteIncomeComboBox->count(); ++i) {
           if (ui->deleteIncomeComboBox->itemData(i).toString() == targetId) {
               ui->deleteIncomeComboBox->removeItem(i);
               break;
           }
       }


       incomeRedoStack.push(lastAddedIncome);



                showMessage("Success", "Recently added income has been  deleted", "success");
            updateBalance();

}

void MenuForm::on_reDelButton_clicked()
{
    if (expenseDelRedoStack.isEmpty()) {

      showMessage("Info", "No recently deleted expense to redo", "info");   return;
     }

    Expense* expense = expenseDelRedoStack.pop();


    QString targetId = expense->getId();
    Expense* deleted = processor->deleteRecordById(expenses, targetId);
    if (!deleted) {

        expenseDelRedoStack.push(expense);
        return;
    }


    for (int i = 0; i < ui->deleteExpenseComboBox->count(); ++i) {
        if (ui->deleteExpenseComboBox->itemData(i).toString() == targetId) {
            ui->deleteExpenseComboBox->removeItem(i);
            break;
        }
    }


    for (int i = 0; i < ui->expenseListWidget->count(); ++i) {
        QListWidgetItem* item = ui->expenseListWidget->item(i);
        if (item->data(Qt::UserRole).toString() == targetId) {
            delete ui->expenseListWidget->takeItem(i);
            break;
        }
    }


    expenseDelUndoStack.push(expense);

    showMessage("Success", "Recently added expense has been deleted", "success");
    updateBalance();
}
void MenuForm::on_pushButton_27_clicked()
{
    if (incomeDelRedoStack.isEmpty()) {
        showMessage("Information", "⚠️ No deleted incomes to restore", "info");
        return;
     }

    Income* income = incomeDelRedoStack.pop();
    QString targetId = income->getId();


    Income* deleted = processor->deleteRecordById(incomes, targetId);
    if (!deleted) {

        incomeDelRedoStack.push(income);
        return;
    }


    for (int i = 0; i < ui->deleteIncomeComboBox->count(); ++i) {
        if (ui->deleteIncomeComboBox->itemData(i).toString() == targetId) {
            ui->deleteIncomeComboBox->removeItem(i);
            break;
        }
    }


    for (int i = 0; i < ui->incomeListWidget->count(); ++i) {
        QListWidgetItem* item = ui->incomeListWidget->item(i);
        if (item->data(Qt::UserRole).toString() == targetId) {
            delete ui->incomeListWidget->takeItem(i);
            break;
        }
    }


    incomeDelUndoStack.push(income);

    showMessage("Success", "Income deletion has been redone", "success");
     updateBalance();
}

void MenuForm::on_pushButton_29_clicked()
{
    auto filtered = processor->getExpensesByCategory(expenses, expenseCategory);



    processor->sortRecordsByDate(filtered, false);
    updateExpensesForCategory(filtered);
    ui->stackedWidget->setCurrentWidget(ui->viewPage);
}

void MenuForm::on_pushButton_28_clicked()
{
    auto filtered = processor->getExpensesByCategory(expenses, expenseCategory);



   processor->sortRecordsBySum(filtered, false);
    updateExpensesForCategory(filtered);
    ui->stackedWidget->setCurrentWidget(ui->viewPage);
}

void MenuForm::on_pushButton_30_clicked()
{
    auto filtered = processor->getIncomesByCategory(incomes, incomeCategory);



    processor->sortRecordsByDate(filtered, false);
    updateIncomesForCategory(filtered);
    ui->stackedWidget->setCurrentWidget(ui->incomeViewPage);
}

void MenuForm::on_pushButton_31_clicked()
{
    auto filtered = processor->getIncomesByCategory(incomes, incomeCategory);



    processor->sortRecordsBySum(filtered, false);
    updateIncomesForCategory(filtered);
    ui->stackedWidget->setCurrentWidget(ui->incomeViewPage);
}

void MenuForm::on_pushButton_24_clicked()
{

}

void MenuForm::on_findButton_clicked()
{
    QString startDateText = ui->lineEdit->text().trimmed();
    QString endDateText = ui->lineEdit_2->text().trimmed();

    // Check if fields are empty
    if (startDateText.isEmpty() || endDateText.isEmpty()) {
        showMessage("Error", "❌ Please enter both start and end dates!", "error");
        return;
    }

    QDate startDate = parseDateFromString(startDateText);
    QDate endDate = parseDateFromString(endDateText);

    if (!startDate.isValid() || !endDate.isValid()) {
        showMessage("Error", "❌ Invalid date format!\nPlease use: DD.MM.YYYY", "error");
        return;
    }

    if (startDate > endDate) {
        showMessage("Error", "❌ Start date cannot be greater than end date!", "error");
        return;
    }


    auto foundExpenses = processor->findRecordsByDateRange(expenses, startDate, endDate);
    auto foundIncomes = processor->findRecordsByDateRange(incomes, startDate, endDate);

    // Check if any records found
    if (foundExpenses.empty() && foundIncomes.empty()) {
        showMessage("Information",
            QString("📭 No records found for period from %1 to %2")
                .arg(startDate.toString("dd.MM.yyyy"))
                .arg(endDate.toString("dd.MM.yyyy")),
            "info");
        return;
    }

    // Sort records
    processor->sortRecordsByDate(foundExpenses, true);
    processor->sortRecordsByDate(foundIncomes, true);

    // Calculate total amounts
    double totalExpenses = processor->calculateTotalAmount(foundExpenses, endDate);
    double totalIncomes = processor->calculateTotalAmount(foundIncomes, endDate);
    double balance = totalIncomes - totalExpenses;

    // Show success message
    showMessage("Success",
        QString("✅ Found %1 expenses and %2 incomes\n"
                "🔴 Total expenses: %3 BYN\n"
                "🟢 Total incomes: %4 BYN\n"
                "💰 Balance: %5 BYN\n"
                "📅 Period: %6 - %7")
            .arg(foundExpenses.size())
            .arg(foundIncomes.size())
            .arg(totalExpenses, 0, 'f', 2)
            .arg(totalIncomes, 0, 'f', 2)
            .arg(balance, 0, 'f', 2)
            .arg(startDate.toString("dd.MM.yyyy"))
            .arg(endDate.toString("dd.MM.yyyy")),
        "success");

    // Show results dialog with tabs
        showList(foundExpenses, foundIncomes);

        ui->lineEdit->clear();
           ui->lineEdit_2->clear();
}

void MenuForm::showList(const std::vector<Expense*>& expenses,  const std::vector<Income*>& incomes)
{
    QDialog dialog(this);
        dialog.setWindowTitle("Found Records");
        dialog.setMinimumSize(700, 600);

        QVBoxLayout *layout = new QVBoxLayout(&dialog);

        // Add title with record count
        QLabel *titleLabel = new QLabel(
            QString("Found records: %1 expenses, %2 incomes")
                .arg(expenses.size())
                .arg(incomes.size())
        );
        layout->addWidget(titleLabel);

        // Tab widget
        QTabWidget *tabWidget = new QTabWidget();
        layout->addWidget(tabWidget);

        // Expenses tab
        QWidget *expensesTab = new QWidget();
        QVBoxLayout *expensesLayout = new QVBoxLayout(expensesTab);

        QListWidget *expensesList = new QListWidget();
        expensesList->setAlternatingRowColors(true);
        expensesLayout->addWidget(expensesList);

        // Fill expenses
        for (const auto& expense : expenses) {
            if (expense) {
                QString itemText = expense->printDisplay();
                QListWidgetItem* item = new QListWidgetItem(itemText);


                if (expense->getAmount() > 1000) {
                    item->setBackground(QBrush(QColor(240, 240, 240)));
                }

                expensesList->addItem(item);
            }
        }

        // Incomes tab
        QWidget *incomesTab = new QWidget();
        QVBoxLayout *incomesLayout = new QVBoxLayout(incomesTab);

        QListWidget *incomesList = new QListWidget();
        incomesList->setAlternatingRowColors(true);
        incomesLayout->addWidget(incomesList);

        // Fill incomes
        for (const auto& income : incomes) {
            if (income) {
                QString itemText = income->printDisplay();
                QListWidgetItem* item = new QListWidgetItem(itemText);

                // Optional: highlight large incomes
                if (income->getAmount() > 1000) {
                    item->setBackground(QBrush(QColor(240, 240, 240)));
                }

                incomesList->addItem(item);
            }
        }

        // Add tabs
        tabWidget->addTab(expensesTab, QString("Expenses (%1)").arg(expenses.size()));
        tabWidget->addTab(incomesTab, QString("Incomes (%1)").arg(incomes.size()));

        // Close button
        QPushButton *closeButton = new QPushButton("Close");
        layout->addWidget(closeButton);
        connect(closeButton, &QPushButton::clicked, &dialog, &QDialog::accept);

        dialog.exec();
}

void MenuForm::on_redoIncomeButton_2_clicked()
{

        if (incomeRedoStack.isEmpty()) {
            showMessage("Information", "⚠️ Nothing to redo for incomes", "info");
            return;
        }

          Income* lastUndoneIncome = incomeRedoStack.pop();


           processor->addRecord(incomes, lastUndoneIncome);


           QString itemText = lastUndoneIncome->printDisplay();


           QListWidgetItem* item = new QListWidgetItem(itemText);
           item->setData(Qt::UserRole, lastUndoneIncome->getId());
           ui->expenseListWidget->addItem(item);


           ui->deleteExpenseComboBox->addItem(itemText, lastUndoneIncome->getId());

           incomeUndoStack.push(lastUndoneIncome);


           showMessage("Success", "Recently deleted income has been  renewed", "success");
           updateBalance();

}

void MenuForm::on_pushButton_25_clicked()
{
    Expense* record = processor->findLargestRecord<Expense>(expenses);
       showFoundRecord(record, "Largest Expense", "Largest expense found");
}
void MenuForm::on_pushButton_32_clicked()
{
    Expense* record = processor->findSmallestRecord<Expense>(expenses);
       showFoundRecord(record, "Smallest Expense", "Smallest expense found");
}

void MenuForm::on_pushButton_34_clicked()
{
    Income* record = processor->findLargestRecord<Income>(incomes);
       showFoundRecord(record, "Largest Income", "Largest income found");
}



void MenuForm::on_pushButton_38_clicked()
{
    Income* record = processor->findSmallestRecord<Income>(incomes);
        showFoundRecord(record, "Smallest Income", "Smallest income found");
}

void MenuForm::on_cancelIncomeButton_2_clicked()
{
    if (incomeDelUndoStack.isEmpty()) {
           showMessage("Info", "No operations to undo", "info");
           return;
       }

       Income* income = incomeDelUndoStack.pop();


       processor->addRecord(incomes, income);


       QString displayText = income->printDisplay();
       ui->deleteIncomeComboBox->addItem(displayText, income->getId());


       QListWidgetItem* item = new QListWidgetItem(displayText);
       item->setData(Qt::UserRole, income->getId());
       ui->incomeListWidget->addItem(item);


       incomeDelRedoStack.push(income);

       showMessage("Success", "Recently deleted income has been renewed", "success");
       updateBalance();
}

void MenuForm::on_pushButton_11_clicked()
{
    QApplication::quit();
}

void MenuForm::on_pushButton_10_clicked()
{
    QApplication::quit();
}


