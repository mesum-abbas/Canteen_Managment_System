#ifndef MANAGE_INVENTORY_H
#define MANAGE_INVENTORY_H

#include <QDialog>
#include <QComboBox>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QSqlDatabase>   
#include <QSqlQuery>      
#include <QSqlError>
#include "add_update.h"

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
    void on_pushButton_clicked();
    void on_pushButton_3_clicked();
     void on_pushButton_2_clicked();
    void on_tableWidget_cellActivated(int row, int column);
    void applyFilter();
    void updateBlinkEffect();
    void loadInventoryData();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::manage_inventory *ui;
    QComboBox *filterComboBox;
    QLabel *filterLabel;
    QPushButton *filterButton;
    QLabel *titleLabel;
    QTimer *blinkTimer;
    bool blinkState;
    add_update *addUpdateWindow;

};

#endif // MANAGE_INVENTORY_H
