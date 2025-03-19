#ifndef USER_AUTHENTICATION_H
#define USER_AUTHENTICATION_H

#include <QDialog>
#include"user.h"

namespace Ui {
class user_authentication;
}

class user_authentication : public QDialog
{
    Q_OBJECT

public:
    explicit user_authentication(QWidget *parent = nullptr);
    ~user_authentication();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::user_authentication *ui;
    user *ptruser;
};

#endif // USER_AUTHENTICATION_H
