#include "user_authentication.h"
#include "ui_user_authentication.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

user_authentication::user_authentication(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::user_authentication)
{
    ui->setupUi(this);
    ptruser = new user();
}

user_authentication::~user_authentication()
{
    delete ui;
    delete ptruser;
}

void user_authentication::on_pushButton_2_clicked()
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    QSqlQuery query;
    query.prepare("SELECT password, role FROM Users WHERE username = :username");
    query.bindValue(":username", username); // username: cashier

    if (!query.exec()) {
        qDebug() << "Login query failed:" << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to verify credentials.");
        return;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        QString role = query.value(1).toString();

        if (password == storedPassword) { // passwrord: cashier123
            ptruser->show();
        } else {
            QMessageBox::warning(this, "Error", "Incorrect password.");
        }
    } else {
        QMessageBox::warning(this, "Error", "User not found.");
    }
}
