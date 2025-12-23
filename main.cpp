#include "mainwindow.h"
#include <QLoggingCategory>
#include <QApplication>

int main(int argc, char *argv[])
{
  QLoggingCategory::setFilterRules("qt.qml.typeregistration.debug=true");
    QApplication a(argc, argv);
    QFont interFont("Inter");
      a.setFont(interFont);
    MainWindow w;
    w.show();
    return a.exec();
}
