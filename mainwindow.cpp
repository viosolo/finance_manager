#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "menuform.h"
#include <QVBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    setFixedSize(788, 601);
    setWindowTitle("Finance Manager");


    setStyleSheet(
        "MainWindow {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #7C3AED, stop:1 #5B21B6);"
        "}"
    );


    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(80, 100, 80, 100);
    mainLayout->setSpacing(30);

    // App icon
    QLabel *iconLabel = new QLabel("💰");
    iconLabel->setStyleSheet(
        "font-size: 120px;"
        "background: transparent;"
        "border: none;"
    );
    iconLabel->setAlignment(Qt::AlignCenter);
    iconLabel->setFixedHeight(140);


    QLabel *titleLabel = new QLabel("Finance Manager");
    titleLabel->setStyleSheet(
        "color: white;"
        "font-size: 42px;"
        "font-weight: bold;"
        "background: transparent;"
        "border: none;"
        "font-family: 'Inter';"
    );
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setFixedHeight(60);


    QLabel *subtitleLabel = new QLabel("Manage your finances\nefficiently and easily");
    subtitleLabel->setStyleSheet(
        "color: rgba(255, 255, 255, 0.9);"
        "font-size: 20px;"
        "background: transparent;"
        "border: none;"
        "font-family: 'Inter';"
    );
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setFixedHeight(60);


    ui->pushButton->setText("🚀 Continue");
    ui->pushButton->setFixedSize(280, 70);
    ui->pushButton->setStyleSheet(
        "QPushButton {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #10B981, stop:1 #059669);"
        "    color: white;"
        "    font-size: 20px;"
        "    font-weight: bold;"
        "    border: none;"
        "    border-radius: 15px;"
        "}"
        "QPushButton:hover {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #34D399, stop:1 #10B981);"
        "}"
        "QPushButton:pressed {"
        "    background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #059669, stop:1 #047857);"
        "}"
    );


    mainLayout->addStretch();
    mainLayout->addWidget(iconLabel);
    mainLayout->addSpacing(10);
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(subtitleLabel);
    mainLayout->addStretch();
    mainLayout->addWidget(ui->pushButton, 0, Qt::AlignCenter);
    mainLayout->addStretch();
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    MenuForm* form = new MenuForm(this);
        form->show();

}
