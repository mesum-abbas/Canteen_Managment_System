#include "admin_dash.h"
#include "ui_admin_dash.h"
admin_dash::admin_dash(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::admin_dash)
{
    ui->setupUi(this);
    this->setStyleSheet("background-color: rgb(245, 245, 220);}");  // Added this line for the new background color
    // Adding hover effect and styling for all QPushButtons with increased font size and bold text
    QString buttonStyle = "QPushButton { color: black; background-color: rgb(255, 191, 60); border-radius: 10px; font-size: 12pt; font-weight: bold; }"
                          "QPushButton:hover { background-color: rgb(255, 215, 80); }";
    ui->pushButton_3->setStyleSheet(buttonStyle);
    ui->pushButton_4->setStyleSheet(buttonStyle);
    ui->pushButton_6->setStyleSheet(buttonStyle);
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
