#ifndef USER_H
#define USER_H

#include <QDialog>
#include"logout.h"

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
    void on_pushButton_3_clicked();

private:
    Ui::user *ui;
    logout *ptrlogout1;

};

#endif // USER_H
