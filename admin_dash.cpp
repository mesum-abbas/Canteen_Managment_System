#include "admin_dash.h"
#include "ui_admin_dash.h"

admin_dash::admin_dash(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::admin_dash)
{
    ui->setupUi(this);
    ptrmanageinventory = new manage_inventory();
    ptrgeneratereport = new gen_report();
    ptrlogout = new logout();

}

admin_dash::~admin_dash()
{
    delete ui;
    delete ptrmanageinventory;
    delete ptrgeneratereport;

    delete ptrlogout;
}




void admin_dash::on_pushButton_3_clicked()
{
    ptrmanageinventory->show();
}


void admin_dash::on_pushButton_4_clicked()
{
    ptrgeneratereport->show();
}


void admin_dash::on_pushButton_6_clicked()
{
    ptrlogout->show();
}
