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

    ptrlogout1 = new logout();

    // Connect ComboBox selection change
    connect(ui->comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onItemSelected()));

    // Setup Table Widget headers
    ui->tableWidget->setColumnCount(4);
    QStringList headers = {"ITEM NAME", "QUANTITY", "NET PRICE", "TOTAL PRICE"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Make stock and price fields read-only
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit_3->setReadOnly(true);

    // Initialize total amount
    totalAmount = 0.0;

    // Load item names from database
    loadItemsFromDatabase();
}

user::~user()
{
    delete ui;
    delete ptrlogout1;
}

// Load item names into comboBox from database
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

// Slot when item is selected in ComboBox
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

// Add item to cart
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

    // Check if the item already exists in the cart
    bool itemFound = false;
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        if (ui->tableWidget->item(i, 0)->text() == itemName) {
            // Update existing row
            int existingQuantity = ui->tableWidget->item(i, 1)->text().toInt();
            double existingSubtotal = ui->tableWidget->item(i, 3)->text().toDouble();

            int newQuantity = existingQuantity + quantity;
            double newSubtotal = price * newQuantity;

            ui->tableWidget->item(i, 1)->setText(QString::number(newQuantity));
            ui->tableWidget->item(i, 3)->setText(QString::number(newSubtotal));

            // Update totalAmount
            totalAmount += (newSubtotal - existingSubtotal);
            ui->lineEdit_4->setText(QString::number(totalAmount));

            itemFound = true;
            break;
        }
    }

    if (!itemFound) {
        // Insert new row
        int rowCount = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(rowCount);

        ui->tableWidget->setItem(rowCount, 0, new QTableWidgetItem(itemName));
        ui->tableWidget->setItem(rowCount, 1, new QTableWidgetItem(QString::number(quantity)));
        ui->tableWidget->setItem(rowCount, 2, new QTableWidgetItem(QString::number(price)));
        ui->tableWidget->setItem(rowCount, 3, new QTableWidgetItem(QString::number(totalPrice)));

        // Update totalAmount
        totalAmount += totalPrice;
        ui->lineEdit_4->setText(QString::number(totalAmount));
    }

    // Clear quantity field
    ui->lineEdit_2->clear();
}

// Confirm order and insert into database
    void user::on_pushButton_clicked() {
    if (ui->tableWidget->rowCount() == 0) {
        QMessageBox::information(this, "Info", "No items in the cart.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query(db);

    // Begin Transaction
    db.transaction();

    // Insert into Orders table
    query.prepare("INSERT INTO Orders (user_id, total_amount) VALUES (:user_id, :total_amount)");
    query.bindValue(":user_id", 2); // Assume logged in user id is 2 (cashier), replace this dynamically later
    query.bindValue(":total_amount", totalAmount);

    if (!query.exec()) {
        qDebug() << "Failed to insert order:" << query.lastError().text();
        db.rollback();
        QMessageBox::critical(this, "Error", "Failed to place order.");
        return;
    }

    int orderId = query.lastInsertId().toInt();

    // Insert into OrderDetails and update stock
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        QString itemName = ui->tableWidget->item(i, 0)->text();
        int quantity = ui->tableWidget->item(i, 1)->text().toInt();
        double price = ui->tableWidget->item(i, 2)->text().toDouble();
        double subtotal = quantity * price;

        // Get item_id
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

        // Insert into OrderDetails
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

    // Commit Transaction
    db.commit();

    QMessageBox::information(this, "Order Confirmed", "Your order has been placed successfully!");

    // Now generate the receipt from database
    QDialog *receiptDialog = new QDialog(this);
    receiptDialog->setWindowTitle("Canteen Receipt");
    receiptDialog->resize(300, 400);

    QVBoxLayout *layout = new QVBoxLayout(receiptDialog);

    QTextEdit *receiptText = new QTextEdit(receiptDialog);
    receiptText->setReadOnly(true);

    QString receipt;
    receipt += "*** CANTEEN RECEIPT ***\n";
    receipt += "----------------------------------------\n";
    receipt += QString("Order ID: %1\n").arg(orderId);
    receipt += "Date/Time: " + QDateTime::currentDateTime().toString("dd-MM-yyyy HH:mm:ss") + "\n";
    receipt += "----------------------------------------\n";
    receipt += QString("%1%2%3%4\n")
                   .arg("Item", -10)
                   .arg("Qty", 6)
                   .arg("Price", 10)
                   .arg("Total", 10);
    receipt += "----------------------------------------\n";

    // Query to fetch order details
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
                           .arg(itemName, -10)
                           .arg(QString::number(quantity), 6)
                           .arg(QString::number(price, 'f', 2), 10)
                           .arg(QString::number(subtotal, 'f', 2), 10);

            calculatedTotal += subtotal;
        }
    } else {
        qDebug() << "Failed to fetch order details for receipt:" << fetchQuery.lastError().text();
    }

    receipt += "----------------------------------------\n";
    receipt += QString("Grand Total:%1\n").arg(QString::number(calculatedTotal, 'f', 2).rightJustified(28));
    receipt += "----------------------------------------\n\n";
    receipt += "    Thank you for your order!";

    receiptText->setText(receipt);
    layout->addWidget(receiptText);
    receiptDialog->setLayout(layout);

    // Show the receipt dialog
    receiptDialog->exec();

    // Clear cart after receipt
    ui->tableWidget->setRowCount(0);
    totalAmount = 0.0;
    ui->lineEdit_4->clear();

    // Reload stock for current item
    onItemSelected();
    }

    void user::on_pushButton_2_clicked() {
        ui->tableWidget->setRowCount(0);
        totalAmount = 0.0;
        ui->lineEdit_4->clear();
    }

    // Logout button
    void user::on_pushButton_3_clicked() {
        ptrlogout1->show();
    }
