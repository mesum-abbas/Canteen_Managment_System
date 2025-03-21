#include "admin_authentication.h"
#include "ui_admin_authentication.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

admin_authentication::admin_authentication(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::admin_authentication)
{
    ui->setupUi(this);
    ptradmindashboard = new admin_dash();
}

admin_authentication::~admin_authentication()
{
    delete ui;
    delete ptradmindashboard;
}

void admin_authentication::on_pushButton_2_clicked()
{
    QString username = ui->lineEdit->text();
    QString password = ui->lineEdit_2->text();

    QSqlQuery query;
    query.prepare("SELECT password FROM Users WHERE username = :username AND role = 'admin'"); // username: admin
    query.bindValue(":username", username); // password: admin123

    if (!query.exec()) {
        qDebug() << "Query Error: " << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to execute query.");
        return;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();

        if (password == storedPassword) {
            ptradmindashboard->show();
            this->close(); // Close authentication dialog after successful login
        } else {
            QMessageBox::warning(this, "Error", "Wrong username or password");
        }
    } else {
        QMessageBox::warning(this, "Error", "Wrong username or password");
    }
}
