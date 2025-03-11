#include "admin_authentication.h"
#include "ui_admin_authentication.h"
#include <QMessageBox>
admin_authentication::admin_authentication(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::admin_authentication)
{
    ui->setupUi(this);
    ptradmindashboard = new admin_dash();
}
admin_authentication::~admin_authentication()
{
    delete ui;
    delete ptradmindashboard;
}
void admin_authentication::on_pushButton_2_clicked()
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    if (username == "Admin" && password == "123") {
        ptradmindashboard->show();
    } else {
        QMessageBox::warning(this, "Error", "Wrong username or password");
    }
}
