#include "user_authentication.h"
#include "ui_user_authentication.h"
#include <QMessageBox>

user_authentication::user_authentication(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::user_authentication)
{
    ui->setupUi(this);
    ptruser = new user();
}

user_authentication::~user_authentication()
{
    delete ui;
    delete ptruser;
}

void user_authentication::on_pushButton_2_clicked()
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();
    if (username == "admin" && password == "1234") {
        ptruser->show();
    } else {
        QMessageBox::warning(this, "Error", "Wrong username or password");
    }
}

