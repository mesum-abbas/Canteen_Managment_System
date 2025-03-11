#ifndef MANAGE_INVENTORY_H
#define MANAGE_INVENTORY_H

#include <QDialog>

namespace Ui {
class manage_inventory;
}

class manage_inventory : public QDialog
{
    Q_OBJECT

public:
    explicit manage_inventory(QWidget *parent = nullptr);
    ~manage_inventory();

private slots:
    void on_tableWidget_cellActivated(int row, int column);

    void on_pushButton_clicked();
    void on_pushButton_3_clicked();



private:
    Ui::manage_inventory *ui;
    bool addingItem;
    bool updatingItem;

};

#endif // MANAGE_INVENTORY_H
