#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ptradminauthentication = new admin_authentication();

}

MainWindow::~MainWindow()
{
    delete ui;
    delete ptradminauthentication;
}

void MainWindow::on_pushButton_clicked()
{
    ptradminauthentication->show();
}

