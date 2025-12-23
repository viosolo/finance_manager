QT       += core gui


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

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
    doublelinkedlist.cpp \
    expense.cpp \
    filemanager.cpp \
    financemanager.cpp \
    financeprocessor.cpp \
    financerecord.cpp \
    finexception.cpp \
    income.cpp \
    main.cpp \
    mainwindow.cpp \
    menuform.cpp \
    recurringexpense.cpp \
    taxedincome.cpp

HEADERS += \
    AmountException.h \
    CategoryException.h \
    FileException.h \
    IntervalException.h \
    TaxException.h \
    doublelinkedlist.h \
    expense.h \
    filemanager.h \
    financeprocessor.h \
    financerecord.h \
    finexception.h \
    income.h \
    mainwindow.h \
    menuform.h \
    recurringexpense.h \
    taxedincome.h

FORMS += \
    mainwindow.ui \
    menuform.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
