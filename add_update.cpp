#include "add_update.h"
#include "ui_add_update.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>

add_update::add_update(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::add_update)
{
    ui->setupUi(this);

    connect(ui->pushButton_2, &QPushButton::clicked, this, &add_update::onSubmit);
    connect(ui->pushButton, &QPushButton::clicked, this, &add_update::close);
}

add_update::~add_update()
{
    delete ui;
}

void add_update::onSubmit()
{
    QString itemName = ui->lineEdit->text().trimmed();
    int stock = ui->lineEdit_2->text().toInt();
    double price = ui->lineEdit_3->text().toDouble();

    if (itemName.isEmpty() || stock < 0 || price <= 0) {
        QMessageBox::warning(this, "Invalid Input", "Please enter valid item details.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Items (item_name, price, stock_quantity, low_stock_threshold) VALUES (?, ?, ?, 10)");
    query.addBindValue(itemName);
    query.addBindValue(price);
    query.addBindValue(stock);

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Item added successfully.");
    } else {
        QMessageBox::critical(this, "Error", "Failed to update item: " + query.lastError().text());
    }
}
