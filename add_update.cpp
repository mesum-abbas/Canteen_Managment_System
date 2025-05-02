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

    // Set window background and text colors
    this->setStyleSheet("QDialog { background-color: #333333; } QLabel { color: white; } QLineEdit { color: black; background-color: white; }");

    // Add specific styling for the labels to make them more visible
    ui->label->setStyleSheet("font-weight: bold; color: yellow; font-size: 18px;");
    ui->label_2->setStyleSheet("font-weight: bold; color: yellow; font-size: 16px;");
    ui->label_3->setStyleSheet("font-weight: bold; color: yellow; font-size: 16px;");
    ui->label_4->setStyleSheet("font-weight: bold; color: yellow; font-size: 16px;");

    // Style the buttons - orange/yellow background, black text, larger font
    ui->pushButton->setStyleSheet("background-color: rgb(255, 191, 60); color: black; padding: 6px; border-radius: 10px; font-size: 14px; font-weight: bold;");
    ui->pushButton_2->setStyleSheet("background-color: rgb(255, 191, 60); color: black; padding: 6px; border-radius: 10px; font-size: 14px; font-weight: bold;");

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
        this->close();
    } else {
        QMessageBox::critical(this, "Error", "Failed to update item: " + query.lastError().text());
    }
}
