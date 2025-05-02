#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ptradminauthentication = new admin_authentication();
    ptruserauthentication = new user_authentication();

    // Add hover effect to pushButton
    ui->pushButton->setStyleSheet("QPushButton { color: black; background-color: rgb(255, 191, 60); }"
                                  "QPushButton:hover { background-color: rgb(255, 215, 80); }");

    // Add hover effect to pushButton_2
    ui->pushButton_2->setStyleSheet("QPushButton { color: black; background-color: rgb(255, 191, 60); }"
                                    "QPushButton:hover { background-color: rgb(255, 215, 80); }");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete ptradminauthentication;
    delete ptruserauthentication;
}

void MainWindow::on_pushButton_clicked()
{
    ptradminauthentication->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    ptruserauthentication->show();
}
