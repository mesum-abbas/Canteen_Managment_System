#ifndef ADMIN_DASH_H
#define ADMIN_DASH_H

#include <QDialog>
#include"manage_inventory.h"
#include"gen_report.h"
#include"stock_alert.h"
#include"logout.h"

namespace Ui {
class admin_dash;
}

class admin_dash : public QDialog
{
    Q_OBJECT

public:
    explicit admin_dash(QWidget *parent = nullptr);
    ~admin_dash();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

private:
    Ui::admin_dash *ui;
    manage_inventory  *ptrmanageinventory;
    gen_report  *ptrgeneratereport;
    stock_alert  *ptrstockalert;
    logout *ptrlogout;

};

#endif // ADMIN_DASH_H
