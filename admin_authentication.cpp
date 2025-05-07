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

    // Modified line edits with larger font (ONLY CHANGE)
    ui->lineEdit->setStyleSheet(
        "QLineEdit {"
        "   color: black;"
        "   border: none;"
        "   border-bottom: 1px solid black;"
        "   font-size: 14px;"  // Increased font size
        "}"
        );
    ui->lineEdit_2->setStyleSheet(
        "QLineEdit {"
        "   color: black;"
        "   border: none;"
        "   border-bottom: 1px solid black;"
        "   font-size: 14px;"  // Increased font size
        "}"
        );

    // Existing button styling (unchanged)
    ui->pushButton_2->setStyleSheet(
        "QPushButton {"
        "  background-color: rgb(255, 191, 60);"
        "   color: black;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: 10px;"
        "   font-weight: bold;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(230, 172, 54);"
        "}"
        );
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
    query.prepare("SELECT password FROM Users WHERE username = :username AND role = 'admin'");
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Query Error: " << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to execute query.");
        return;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();

        if (password == storedPassword) {
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
            ptradmindashboard->show();
            this->close();
        } else {
            QMessageBox::warning(this, "Error", "Wrong username or password");
        }
    } else {
        QMessageBox::warning(this, "Error", "Wrong username or password");
    }
}

