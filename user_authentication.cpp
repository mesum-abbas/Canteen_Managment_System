#include "user_authentication.h"
#include "ui_user_authentication.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QToolButton>
#include <QIcon>

user_authentication::user_authentication(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::user_authentication)
{
    ui->setupUi(this);
    ptruser = new user();

    // Style the line edits with larger font (ONLY CHANGE)
    ui->lineEdit->setStyleSheet(
        "color: black;"
        "border: none;"
        "border-bottom: 1px solid black;"
        "padding-right: 25px;"
        "font-size: 14px;"  // Increased font size
        );
    ui->lineEdit_2->setStyleSheet(
        "color: black;"
        "border: none;"
        "border-bottom: 1px solid black;"
        "padding-right: 25px;"
        "font-size: 14px;"  // Increased font size
        );

    // Rest of the existing code remains exactly the same
    QToolButton *togglePasswordButton = new QToolButton(this);
    togglePasswordButton->setCursor(Qt::PointingHandCursor);
    togglePasswordButton->setIcon(QIcon(":/icons/eye_closed.png"));
    togglePasswordButton->setStyleSheet("background: transparent; border: none;");
    togglePasswordButton->setCheckable(true);

    QSize lineEditSize = ui->lineEdit_2->size();
    togglePasswordButton->move(ui->lineEdit_2->x() + lineEditSize.width() - 30,
                               ui->lineEdit_2->y() + (lineEditSize.height() - 20) / 2);

    connect(togglePasswordButton, &QToolButton::toggled, [=](bool checked) {
        if (checked) {
            togglePasswordButton->setIcon(QIcon(":/icons/eye_open.png"));
            ui->lineEdit_2->setEchoMode(QLineEdit::Normal);
        } else {
            togglePasswordButton->setIcon(QIcon(":/icons/eye_closed.png"));
            ui->lineEdit_2->setEchoMode(QLineEdit::Password);
        }
    });

    ui->pushButton_2->setStyleSheet(
        "QPushButton {"
        "   background-color: rgb(255, 191, 60);"
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
    query.bindValue(":username", username);

    if (!query.exec()) {
        qDebug() << "Login query failed:" << query.lastError().text();
        QMessageBox::critical(this, "Database Error", "Failed to verify credentials.");
        return;
    }

    if (query.next()) {
        QString storedPassword = query.value(0).toString();
        QString role = query.value(1).toString();

        if (password == storedPassword) {
            ui->lineEdit->clear();
            ui->lineEdit_2->clear();
            ptruser->show();
        } else {
            QMessageBox::warning(this, "Error", "Incorrect password.");
        }
    } else {
        QMessageBox::warning(this, "Error", "User not found.");
    }
}
