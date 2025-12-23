#include "../headers/filemanager.h"
#include <QRegularExpression>
#include "../headers/recurringexpense.h"
#include <QFile>


FileManager::FileManager(const QString& incomeFilePath, const QString& expenseFilePath)
    : incomePath(incomeFilePath), expensePath(expenseFilePath)
{

    openFile(incomeFile, incomePath);
    openFile(expenseFile, expensePath);
}

bool FileManager::openFile(QFile& file, const QString& path) {
    file.setFileName(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    return true;
}



FileManager::~FileManager() {

    if (incomeFile.isOpen()) incomeFile.close();
    if (expenseFile.isOpen()) expenseFile.close();
}
void FileManager::setIncomePath(const QString& path) {
    incomePath = path;
}

void FileManager::setExpensePath(const QString& path) {
    expensePath = path;
}

QString FileManager::getIncomePath() const {
    return incomePath;
}

QString FileManager::getExpensePath() const {
    return expensePath;
}



void FileManager::loadIncomes( DoubleLinkedList<Income*>& list) {
    if (!incomeFile.isOpen()) {
          throw FileException("Income file is not open: " + incomePath);
     }


       incomeFile.seek(0);

       QTextStream in(&incomeFile);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;


        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

        if (parts.size() < 3) {
            qWarning() << "Неверный формат строки:" << line;
            continue;
        }

        // 1. Дата
        QDate date = QDate::fromString(parts[0], "dd.MM.yyyy");
        if (!date.isValid()) {
            qWarning() << "Неверная дата:" << parts[0];
            continue;
        }

        // 2. Сумма
        bool ok = false;
        double amount = parts[1].toDouble(&ok);
        if (!ok) {
            qWarning() << "Неверная сумма:" << parts[1];
            continue;
        }


        QString description;
        int i = 2;
        while (i < parts.size() && !parts[i].startsWith("ID:")) {
            if (!description.isEmpty()) description += " ";
            description += parts[i];
            i++;
        }

        if (i >= parts.size() || !parts[i].startsWith("ID:")) {
            qWarning() << "Отсутствует блок 'ID:' в строке:" << line;
            continue;
        }


        QString uuid = parts[i].mid(3);
        i++;


        bool isTaxed = false;
        double taxRate = 0.0;
        if (i < parts.size() && parts[i] == "Taxed") {
            isTaxed = true;
            i++;
            if (i < parts.size()) {
                taxRate = parts[i].toDouble(&ok);
                if (!ok) {
                    qWarning() << "Неверная налоговая ставка:" << parts[i];
                    taxRate = 0.0;
                }
            }
        }


        IncomeCategory category = Income::stringToCategory(description);


        Income* record = nullptr;
        if (isTaxed) {
            record = new TaxedIncome(amount, date, category, taxRate);
        } else {
            record = new Income(amount, date, category);
        }


        record->setId(uuid);


        list.push_back(record);
    }



}

void FileManager::loadExpenses(DoubleLinkedList<Expense*>& list) {
    if (!expenseFile.isOpen()) {
         throw FileException("Expense file is not open: " + expensePath);

        }

        expenseFile.seek(0);

        QTextStream in(&expenseFile);
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        QStringList parts = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);

        if (parts.size() < 4) {
            qWarning() << "Неверный формат строки:" << line;
            continue;
        }
        QDate date = QDate::fromString(parts[0], "dd.MM.yyyy");
        if (!date.isValid()) {
            qWarning() << "Неверная дата:" << parts[0];
            continue;
        }


        bool ok = false;
        double amount = parts[1].toDouble(&ok);
        if (!ok) {
            qWarning() << "Неверная сумма:" << parts[1];
            continue;
        }


        QString description;
        int i = 2;
        while (i < parts.size() && !parts[i].startsWith("ID:")) {
            if (!description.isEmpty()) description += " ";
            description += parts[i];
            i++;
        }

        if (i >= parts.size() || !parts[i].startsWith("ID:")) {
            qWarning() << "Отсутствует блок 'ID:' в строке:" << line;
            continue;
        }


        QString uuid = parts[i].mid(3);
        i++;


        bool isRecurring = false;
        int recurrenceDays = 0;
        if (i < parts.size() && parts[i] == "Recurring") {
            isRecurring = true;
            i++;
            if (i < parts.size()) {
                recurrenceDays = parts[i].toInt(&ok);
                if (!ok) {
                    qWarning() << "Некорректное значение дней повторения:" << parts[i];
                    recurrenceDays = 0;
                }
            }
        }


        ExpenseCategory category = Expense::stringToCategory(description);


        Expense* record = nullptr;
        if (isRecurring) {
            record = new RecurringExpense(amount, date, category, recurrenceDays);
        } else {
            record = new Expense(amount, date, category);
        }


        record->setId(uuid);


        list.push_back(record);
    }


}

