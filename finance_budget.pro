QT       += core gui

CONFIG += resources_big c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    source/doublelinkedlist.cpp \
    source/expense.cpp \
    source/filemanager.cpp \
    source/financemanager.cpp \
    source/financeprocessor.cpp \
    source/financerecord.cpp \
    source/finexception.cpp \
    source/income.cpp \
    source/main.cpp \
    source/mainwindow.cpp \
    source/menuform.cpp \
    source/recurringexpense.cpp \
    source/taxedincome.cpp

HEADERS += \
    headers/AmountException.h \
    headers/CategoryException.h \
    headers/FileException.h \
    headers/IntervalException.h \
    headers/TaxException.h \
    headers/doublelinkedlist.h \
    headers/expense.h \
    headers/filemanager.h \
    headers/financeprocessor.h \
    headers/financerecord.h \
    headers/finexception.h \
    headers/income.h \
    headers/mainwindow.h \
    headers/menuform.h \
    headers/recurringexpense.h \
    headers/taxedincome.h

FORMS += \
    ui/mainwindow.ui \
    ui/menuform.ui
RESOURCES +=  \
    resourses.qrc
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
