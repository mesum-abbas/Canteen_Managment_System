#ifndef USER_H
#define USER_H

#include <QDialog>
#include "logout.h"

namespace Ui {
class user;
}

class user : public QDialog
{
    Q_OBJECT

public:
    explicit user(QWidget *parent = nullptr);
    ~user();

private slots:
    void on_pushButton_3_clicked();   // Logout button
    void on_pushButton_4_clicked();   // Add to cart button
    void on_pushButton_clicked();     // Confirm order button
    void on_pushButton_2_clicked();   // Cancel order button
    void onItemSelected();            // ComboBox item selected

private:
    Ui::user *ui;
    logout *ptrlogout1;
    double totalAmount;

    void loadItemsFromDatabase(); // Helper function to load items into comboBox
};

#endif // USER_H
