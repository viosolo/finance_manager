#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "financerecord.h"
#include "income.h"
#include "taxedincome.h"
#include "expense.h"
#include "doublelinkedlist.h"
#include "FileException.h"

class FileManager {
private:
    QString incomePath;
     QString expensePath;
     QFile incomeFile;
        QFile expenseFile;

public:
    FileManager(const QString& incomeFilePath, const QString& expenseFilePath);
~FileManager();
void loadIncomes(DoubleLinkedList<Income*>& list) ;
void loadExpenses( DoubleLinkedList<Expense*>& list) ;


    void setIncomePath(const QString& path);
    void setExpensePath(const QString& path);



      bool openFile(QFile& file, const QString& path) ;


    template<typename RecordType>
    void saveToFile(DoubleLinkedList<RecordType*>& records) {
        QString path;

        if constexpr (std::is_same<RecordType, Expense>::value) {
            path = expensePath;
        } else if constexpr (std::is_same<RecordType, Income>::value) {
            path = incomePath;
        }
        else {

            throw FileException("Unknown type for saving");
        }

        QFile file(path);
        if (file.isOpen()) {
                file.close();
            }
        if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            throw FileException("Cannot open file for writing: " + path);
        }

        QTextStream out(&file);
        typename DoubleLinkedList<RecordType*>::Iterator it = records.begin();
        while (it != records.end()) {
            RecordType* record = *it;

            out << *record;
            qDebug() << "→ Проверка записи:"
                     << "Дата:" << record->getDate()
                     << "Сумма:" << record->getAmount()

                     << "ID:" << record->getId();

            ++it;
        }



    }


    QString getIncomePath() const;
    QString getExpensePath() const;
};

#endif // FILEMANAGER_H

