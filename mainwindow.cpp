#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ptradminauthentication = new admin_authentication();
    ptruserauthentication = new user_authentication();

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
