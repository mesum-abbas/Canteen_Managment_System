#ifndef ADMIN_AUTHENTICATION_H
#define ADMIN_AUTHENTICATION_H

#include <QDialog>
#include"admin_dash.h"


namespace Ui {
class admin_authentication;
}

class admin_authentication : public QDialog
{
    Q_OBJECT

public:
    explicit admin_authentication(QWidget *parent = nullptr);
    ~admin_authentication();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::admin_authentication *ui;
    admin_dash *ptradmindashboard;

};

#endif // ADMIN_AUTHENTICATION_H
