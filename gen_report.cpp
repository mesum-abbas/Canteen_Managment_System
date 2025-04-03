
#include "gen_report.h"
#include "ui_gen_report.h"

gen_report::gen_report(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::gen_report)
{
    ui->setupUi(this);
}

gen_report::~gen_report()
{
    delete ui;
}
void gen_report::on_pushButton_2_clicked()
{

    this->close();
}
