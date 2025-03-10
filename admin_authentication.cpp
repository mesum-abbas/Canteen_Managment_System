#include "admin_authentication.h"
#include "ui_admin_authentication.h"

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
    ptradmindashboard->show();
}

