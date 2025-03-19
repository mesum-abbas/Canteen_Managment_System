#include "user.h"
#include "ui_user.h"

user::user(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::user)
{
    ui->setupUi(this);
    ptrlogout1 = new logout();

}

user::~user()
{
    delete ui;
    delete ptrlogout1;
}

void user::on_pushButton_3_clicked()
{
    ptrlogout1->show();
}

