#include "gen_report.h"
#include "ui_gen_report.h"
#include <QMessageBox>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
gen_report::gen_report(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::gen_report)
{
    ui->setupUi(this);
    // Set default date range to today's date
    QDate currentDate = QDate::currentDate();
    ui->dateEdit->setDate(currentDate);
    ui->dateEdit_2->setDate(currentDate);

    // Style the push buttons with hover effect
    QString buttonStyle = "QPushButton { color: black; background-color: rgb(255, 191, 60); border-radius: 10px; font-size: 12pt; font-weight: bold; }"
                          "QPushButton:hover { background-color: rgb(255, 215, 80); }";
    ui->pushButton->setStyleSheet(buttonStyle);
    ui->pushButton_2->setStyleSheet(buttonStyle);

    // Make text in line edits bold
    QString lineEditStyle = "QLineEdit { font-weight: bold; }";
    ui->lineEdit->setStyleSheet(lineEditStyle);
    ui->lineEdit_2->setStyleSheet(lineEditStyle);
    ui->lineEdit_3->setStyleSheet(lineEditStyle);

    // Initialize DB connection
    if (!QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("inventory.db");
    } else {
        db = QSqlDatabase::database();
    }
    if (!db.open()) {
        QMessageBox::critical(this, "Database Error", db.lastError().text());
    }
}
gen_report::~gen_report()
{
    delete ui;
}
// Exit button
void gen_report::on_pushButton_2_clicked()
{
    // Clean UI fields
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit_3->clear();

    // Reset dates
    QDate currentDate = QDate::currentDate();
    ui->dateEdit->setDate(currentDate);
    ui->dateEdit_2->setDate(currentDate);

    this->close();
}
// Generate report button
void gen_report::on_pushButton_clicked()
{
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Error", "Database not connected.");
        return;
    }
    // Get the selected dates
    QDate fromDate = ui->dateEdit->date();
    QDate toDate = ui->dateEdit_2->date();
    // Validate date range
    if (fromDate > toDate) {
        QMessageBox::warning(this, "Invalid Date Range", "The 'From' date must be earlier than the 'To' date.");
        return;
    }
    QString fromDateStr = fromDate.toString("yyyy-MM-dd");
    QString toDateStr = toDate.toString("yyyy-MM-dd");
    // Fetch Total Sales and Total Orders
    double totalSales = 0.0;
    int totalOrders = 0;
    QSqlQuery query;
    query.prepare("SELECT SUM(total_amount), COUNT(*) FROM Orders WHERE date(order_date) BETWEEN :fromDate AND :toDate");
    query.bindValue(":fromDate", fromDateStr);
    query.bindValue(":toDate", toDateStr);
    if (query.exec() && query.next()) {
        totalSales = query.value(0).toDouble();
        totalOrders = query.value(1).toInt();
    } else {
        QMessageBox::warning(this, "Query Error", query.lastError().text());
        return;
    }
    // Fetch Top Selling Item
    QString topItem;
    QSqlQuery topQuery;
    topQuery.prepare(R"(
        SELECT Items.item_name, SUM(OrderDetails.quantity) AS total_quantity
        FROM OrderDetails
        JOIN Orders ON Orders.order_id = OrderDetails.order_id
        JOIN Items ON Items.item_id = OrderDetails.item_id
        WHERE date(Orders.order_date) BETWEEN :fromDate AND :toDate
        GROUP BY Items.item_name
        ORDER BY total_quantity DESC
        LIMIT 1
    )");
    topQuery.bindValue(":fromDate", fromDateStr);
    topQuery.bindValue(":toDate", toDateStr);
    if (topQuery.exec() && topQuery.next()) {
        topItem = topQuery.value(0).toString();
    } else {
        topItem = "N/A"; // No sales during this period
    }
    // Update the LineEdits
    ui->lineEdit->setText(QString("Rs%1").arg(totalSales, 0, 'f', 2));
    ui->lineEdit_3->setText(QString::number(totalOrders));
    ui->lineEdit_2->setText(topItem);
}
