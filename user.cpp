#include "user.h"
#include "ui_user.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QTextEdit>
#include <QVBoxLayout>
#include <QDateTime>

user::user(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::user)
{
    ui->setupUi(this);

    // Line edit styling (your existing changes)
    ui->lineEdit->setStyleSheet("color: #fff261;");
    ui->lineEdit_2->setStyleSheet("color: #fff261;");
    ui->lineEdit_3->setStyleSheet("color: #fff261;");
    ui->lineEdit_4->setStyleSheet("color: #fff261;");

    // Button styling (new changes)
    QString buttonStyle =
        "QPushButton {"
        "   background-color: rgb(255, 191, 60);"
        "   color: black;"
        "   border: none;"
        "   padding: 8px 16px;"
        "   border-radius: 10px;"
        "   font: bold 12pt;"
        "}"
        "QPushButton:hover {"
        "   background-color: rgb(230, 172, 54);"
        "}";

    ui->pushButton->setStyleSheet(buttonStyle);    // Confirm Order
    ui->pushButton_2->setStyleSheet(buttonStyle);  // Clear Cart
    ui->pushButton_3->setStyleSheet(buttonStyle);  // Logout
    ui->pushButton_4->setStyleSheet(buttonStyle);  // Add to Cart

    ptrlogout1 = new logout();

    // Rest of your existing initialization
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onItemSelected()));

    ui->tableWidget->setColumnCount(4);
    QStringList headers = {"ITEM NAME", "QUANTITY", "NET PRICE", "TOTAL PRICE"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->lineEdit->setReadOnly(true);
    ui->lineEdit_3->setReadOnly(true);

    totalAmount = 0.0;
    loadItemsFromDatabase();
}

user::~user()
{
    delete ui;
    delete ptrlogout1;
}

void user::loadItemsFromDatabase()
{
    QSqlQuery query;
    if (query.exec("SELECT item_name FROM Items")) {
        while (query.next()) {
            QString itemName = query.value(0).toString();
            ui->comboBox->addItem(itemName);
        }
    } else {
        qDebug() << "Error loading items:" << query.lastError().text();
    }
}

void user::onItemSelected()
{
    QString item = ui->comboBox->currentText();
    if (item.isEmpty())
        return;

    QSqlQuery query;
    query.prepare("SELECT stock_quantity, price FROM Items WHERE item_name = :item");
    query.bindValue(":item", item);

    if (query.exec() && query.next()) {
        int stock = query.value(0).toInt();
        double price = query.value(1).toDouble();

        ui->lineEdit->setText(QString::number(stock));
        ui->lineEdit_3->setText(QString::number(price));
    } else {
        qDebug() << "Error fetching item details:" << query.lastError().text();
    }
}

void user::on_pushButton_4_clicked()
{
    QString itemName = ui->comboBox->currentText();
    QString stockStr = ui->lineEdit->text();
    QString priceStr = ui->lineEdit_3->text();
    QString quantityStr = ui->lineEdit_2->text();

    if (itemName.isEmpty() || stockStr.isEmpty() || priceStr.isEmpty() || quantityStr.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please fill all fields correctly.");
        return;
    }

    int stock = stockStr.toInt();
    int quantity = quantityStr.toInt();
    double price = priceStr.toDouble();

    if (quantity <= 0) {
        QMessageBox::warning(this, "Warning", "Invalid quantity.");
        return;
    }

    if (quantity > stock) {
        QMessageBox::warning(this, "Warning", "Quantity exceeds available stock!");
        return;
    }

    double totalPrice = quantity * price;

    bool itemFound = false;
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        if (ui->tableWidget->item(i, 0)->text() == itemName) {
            int existingQuantity = ui->tableWidget->item(i, 1)->text().toInt();
            double existingSubtotal = ui->tableWidget->item(i, 3)->text().toDouble();

            int newQuantity = existingQuantity + quantity;
            double newSubtotal = price * newQuantity;

            ui->tableWidget->item(i, 1)->setText(QString::number(newQuantity));
            ui->tableWidget->item(i, 3)->setText(QString::number(newSubtotal));

            totalAmount += (newSubtotal - existingSubtotal);
            ui->lineEdit_4->setText(QString::number(totalAmount));

            itemFound = true;
            break;
        }
    }

    if (!itemFound) {
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowCount);

        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(itemName));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(QString::number(quantity)));
        ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(QString::number(price)));
        ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(QString::number(totalPrice)));

        totalAmount += totalPrice;
        ui->lineEdit_4->setText(QString::number(totalAmount));
    }

    ui->lineEdit_2->clear();
}

void user::on_pushButton_clicked()
{
    if (ui->tableWidget->rowCount() == 0) {
        QMessageBox::information(this, "Info", "No items in the cart.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    db.transaction();

    query.prepare("INSERT INTO Orders (user_id, total_amount) VALUES (:user_id, :total_amount)");
    query.bindValue(":user_id", 2);
    query.bindValue(":total_amount", totalAmount);

    if (!query.exec()) {
        qDebug() << "Failed to insert order:" << query.lastError().text();
        db.rollback();
        QMessageBox::critical(this, "Error", "Failed to place order.");
        return;
    }

    int orderId = query.lastInsertId().toInt();

    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        QString itemName = ui->tableWidget->item(i, 0)->text();
        int quantity = ui->tableWidget->item(i, 1)->text().toInt();
        double price = ui->tableWidget->item(i, 2)->text().toDouble();
        double subtotal = quantity * price;

        QSqlQuery itemQuery;
        itemQuery.prepare("SELECT item_id FROM Items WHERE item_name = :itemName");
        itemQuery.bindValue(":itemName", itemName);

        if (!itemQuery.exec() || !itemQuery.next()) {
            qDebug() << "Failed to find item id:" << itemQuery.lastError().text();
            db.rollback();
            QMessageBox::critical(this, "Error", "Failed to find item id.");
            return;
        }
        int itemId = itemQuery.value(0).toInt();

        QSqlQuery detailQuery;
        detailQuery.prepare("INSERT INTO OrderDetails (order_id, item_id, quantity, subtotal) "
                            "VALUES (:order_id, :item_id, :quantity, :subtotal)");
        detailQuery.bindValue(":order_id", orderId);
        detailQuery.bindValue(":item_id", itemId);
        detailQuery.bindValue(":quantity", quantity);
        detailQuery.bindValue(":subtotal", subtotal);

        if (!detailQuery.exec()) {
            qDebug() << "Failed to insert order detail:" << detailQuery.lastError().text();
            db.rollback();
            QMessageBox::critical(this, "Error", "Failed to place order details.");
            return;
        }
    }

    db.commit();

    QMessageBox::information(this, "Order Confirmed", "Your order has been placed successfully!");

    QDialog *receiptDialog = new QDialog(this);
    receiptDialog->setWindowTitle("Canteen Receipt");
    receiptDialog->resize(400, 500);

    QVBoxLayout *layout = new QVBoxLayout(receiptDialog);

    QTextEdit *receiptText = new QTextEdit(receiptDialog);
    receiptText->setReadOnly(true);
    receiptText->setStyleSheet(
        "color: #f5f5dc;"
        "font-family: Arial;"
        "font-size: 12pt;"
        "font-weight: bold;"
        "background-color: #333333;"
        );

    receiptText->document()->setDocumentMargin(20);
    receiptText->setFrameStyle(QFrame::NoFrame);

    QString receipt;
    receipt += "****** CANTEEN RECEIPT ******\n";
    receipt += "----------------------------------------\n";
    receipt += QString("Order ID: %1\n").arg(orderId);
    receipt += "Date/Time: " + QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss") + "\n";
    receipt += "----------------------------------------\n";
    receipt += QString("%1%2%3%4\n")
                   .arg("Item", -15)
                   .arg("Qty", 8)
                   .arg("Price", 12)
                   .arg("Total", 12);
    receipt += "----------------------------------------\n";

    QSqlQuery fetchQuery;
    fetchQuery.prepare(R"(
        SELECT Items.item_name, OrderDetails.quantity, Items.price, OrderDetails.subtotal
        FROM OrderDetails
        JOIN Items ON OrderDetails.item_id = Items.item_id
        WHERE OrderDetails.order_id = :order_id
    )");
    fetchQuery.bindValue(":order_id", orderId);

    double calculatedTotal = 0.0;

    if (fetchQuery.exec()) {
        while (fetchQuery.next()) {
            QString itemName = fetchQuery.value(0).toString();
            int quantity = fetchQuery.value(1).toInt();
            double price = fetchQuery.value(2).toDouble();
            double subtotal = fetchQuery.value(3).toDouble();

            receipt += QString("%1%2%3%4\n")
                           .arg(itemName, -15)
                           .arg(QString::number(quantity), 8)
                           .arg(QString::number(price, 'f', 2), 12)
                           .arg(QString::number(subtotal, 'f', 2), 12);

            calculatedTotal += subtotal;
        }
    }

    receipt += "----------------------------------------\n";
    receipt += QString("Grand Total:%1\n").arg(QString::number(calculatedTotal, 'f', 2).rightJustified(35));
    receipt += "----------------------------------------\n\n";
    receipt += "    Thank you for your order!";

    receiptText->setText(receipt);
    layout->addWidget(receiptText);
    receiptDialog->setLayout(layout);
    receiptDialog->exec();

    ui->tableWidget->setRowCount(0);
    totalAmount = 0.0;
    ui->lineEdit_4->clear();
    onItemSelected();
}

void user::on_pushButton_2_clicked()
{
    ui->tableWidget->setRowCount(0);
    totalAmount = 0.0;
    ui->lineEdit_4->clear();
}

void user::on_pushButton_3_clicked()
{
    ptrlogout1->show();
}
