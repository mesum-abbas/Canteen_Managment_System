#include "manage_inventory.h"
#include "ui_manage_inventory.h"


manage_inventory::manage_inventory(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::manage_inventory)
{
    ui->setupUi(this);
}

manage_inventory::~manage_inventory()
{
    delete ui;

}

void manage_inventory::on_tableWidget_cellActivated(int row, int column)
{

}


void manage_inventory::on_pushButton_2_clicked()
{
}

