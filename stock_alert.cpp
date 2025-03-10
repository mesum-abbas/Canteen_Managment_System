#include "stock_alert.h"
#include "ui_stock_alert.h"

stock_alert::stock_alert(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::stock_alert)
{
    ui->setupUi(this);
}

stock_alert::~stock_alert()
{
    delete ui;
}
