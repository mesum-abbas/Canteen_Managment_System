#include "add_update.h"
#include "ui_add_update.h"

add_update::add_update(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::add_update)
{
    ui->setupUi(this);
}

add_update::~add_update()
{
    delete ui;
}
