#include "manage_inventory.h"
#include "ui_manage_inventory.h"
#include "add_update.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QSqlQuery>
#include <QSqlError>
#include <QHBoxLayout>
#include <QComboBox>
#include <QLabel>
#include <QFont>
#include <QTimer>
#include <QHeaderView>

manage_inventory::manage_inventory(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::manage_inventory)
    , blinkState(false)
{
    ui->setupUi(this);

    this->resize(1000, 700);
    ui->tableWidget->setGeometry(20, 70, width() * 0.85, height() * 0.75);

    ui->tableWidget->setColumnCount(3);
    QStringList headers = {"Item Name", "Price", "Stock"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    ui->tableWidget->setColumnWidth(2, 200);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    titleLabel = new QLabel("Manage Inventory", this);
    titleLabel->setGeometry(20, 20, 120, 25);
    QFont titleFont = titleLabel->font();
    titleFont.setBold(true);
    titleLabel->setFont(titleFont);
    titleLabel->setStyleSheet("color: white;");

    filterLabel = new QLabel("Sort by:", this);
    filterLabel->setGeometry(150, 20, 50, 25);
    filterLabel->setStyleSheet("color: white;");

    filterComboBox = new QComboBox(this);
    filterComboBox->setGeometry(205, 20, 200, 25);
    filterComboBox->addItem("Item Name (Ascending)");
    filterComboBox->addItem("Item Name (Descending)");
    filterComboBox->addItem("Stock (Ascending)");
    filterComboBox->addItem("Stock (Descending)");
    filterComboBox->setStyleSheet("color: black; background-color: white;");

    filterButton = new QPushButton("Apply Filter", this);
    filterButton->setGeometry(415, 20, 100, 25);
    filterButton->setStyleSheet("color: white; background-color: #2a82da;");
    connect(filterButton, &QPushButton::clicked, this, &manage_inventory::applyFilter);

    // Removing the cell activated connection to eliminate the popup
    // connect(ui->tableWidget, &QTableWidget::cellActivated, this, &manage_inventory::on_tableWidget_cellActivated);

    blinkTimer = new QTimer(this);
    connect(blinkTimer, &QTimer::timeout, this, &manage_inventory::updateBlinkEffect);
    blinkTimer->start(1000);

    loadInventoryData();
}

manage_inventory::~manage_inventory()
{
    delete ui;
    delete filterLabel;
    delete filterComboBox;
    delete filterButton;
    delete titleLabel;

    if (blinkTimer) {
        blinkTimer->stop();
        delete blinkTimer;
    }
}

void manage_inventory::updateBlinkEffect()
{
    blinkState = !blinkState;

    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QTableWidgetItem* stockItem = ui->tableWidget->item(row, 2); // Stock column

        if (stockItem) {
            bool ok;
            int stock = stockItem->text().toInt(&ok);

            if (ok && stock < 10) {
                for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
                    QTableWidgetItem* item = ui->tableWidget->item(row, col);
                    if (item) {
                        if (blinkState) {
                            QColor lightRed(255, 0, 0, 100);
                            item->setBackground(lightRed);
                            item->setForeground(Qt::white); // Change to white for better visibility on red background

                            if (col == 2) {
                                item->setText(QString::number(stock) + " - Low stock please update");
                            }
                        } else {
                            item->setBackground(Qt::white);
                            item->setForeground(Qt::black);

                            if (col == 2) {
                                item->setText(QString::number(stock));
                            }
                        }
                    }
                }
            }
        }
    }
}

void manage_inventory::loadInventoryData()
{
    ui->tableWidget->setRowCount(0); // Clear table before loading

    QSqlQuery query("SELECT item_name, price, stock_quantity FROM Items");
    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to fetch inventory data: " + query.lastError().text());
        return;
    }

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // Item Name
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(query.value(1).toDouble()))); // Price
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(query.value(2).toInt()))); // Stock

        // Set default colors for all items
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = ui->tableWidget->item(row, col);
            if (item) {
                item->setForeground(Qt::white); // Default text color
                item->setBackground(Qt::black); // Default background
            }
        }

        row++;
    }
}


void manage_inventory::on_pushButton_clicked()
{
    addUpdateWindow = new add_update(this);
    addUpdateWindow->exec();
    loadInventoryData();
}

void manage_inventory::on_pushButton_3_clicked()
{
    QDialog updateDialog(this);
    updateDialog.setWindowTitle("Update Items");
    updateDialog.resize(600, 500);

    QVBoxLayout *layout = new QVBoxLayout(&updateDialog);

    QTableWidget *updateTable = new QTableWidget(&updateDialog);
    updateTable->setColumnCount(3);
    updateTable->setHorizontalHeaderLabels({"Item Name", "Price", "Stock"});
    updateTable->setEditTriggers(QAbstractItemView::AllEditTriggers);
    updateTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Set the table's background and text colors for the update dialog
    updateTable->setStyleSheet("QTableWidget { background-color: black; color: white; } "
                               "QTableWidget::item { color: black; background-color: white; }");

    QSqlQuery query("SELECT item_name, price, stock_quantity FROM Items");
    int row = 0;
    while (query.next()) {
        updateTable->insertRow(row);
        updateTable->setItem(row, 0, new QTableWidgetItem(query.value(0).toString())); // Item Name
        updateTable->setItem(row, 1, new QTableWidgetItem(QString::number(query.value(1).toDouble()))); // Price
        updateTable->setItem(row, 2, new QTableWidgetItem(QString::number(query.value(2).toInt()))); // Stock

        // Make item name non-editable
        updateTable->item(row, 0)->setFlags(updateTable->item(row, 0)->flags() & ~Qt::ItemIsEditable);

        // Ensure text is visible
        for (int col = 0; col < updateTable->columnCount(); ++col) {
            QTableWidgetItem* item = updateTable->item(row, col);
            if (item) {
                item->setForeground(Qt::white);
                item->setBackground(Qt::black);
            }
        }

        row++;
    }

    QPushButton *deleteButton = new QPushButton("Delete Selected Row", &updateDialog);
    QPushButton *editButton = new QPushButton("Edit Selected Row", &updateDialog);
    QPushButton *saveButton = new QPushButton("Save Changes", &updateDialog);

    QString buttonStyle = "color: white; background-color: #2a82da;";
    deleteButton->setStyleSheet(buttonStyle);
    editButton->setStyleSheet(buttonStyle);
    saveButton->setStyleSheet(buttonStyle);

    QObject::connect(editButton, &QPushButton::clicked, [&]() {
        int selectedRow = updateTable->currentRow();
        if (selectedRow < 0) {
            QMessageBox::warning(&updateDialog, "Warning", "No row selected!");
            return;
        }

        QString itemName = updateTable->item(selectedRow, 0)->text();
        bool ok;
        double newPrice = QInputDialog::getDouble(&updateDialog, "Edit Price", "Enter new price:",
                                                  updateTable->item(selectedRow, 1)->text().toDouble(), 0, 100000, 2, &ok);
        if (!ok) return;

        int newStock = QInputDialog::getInt(&updateDialog, "Edit Stock", "Enter new stock:",
                                            updateTable->item(selectedRow, 2)->text().toInt(), 0, 100000, 1, &ok);
        if (!ok) return;

        updateTable->item(selectedRow, 1)->setText(QString::number(newPrice));
        updateTable->item(selectedRow, 2)->setText(QString::number(newStock));

        // Ensure text remains visible after editing
        updateTable->item(selectedRow, 1)->setForeground(Qt::black);
        updateTable->item(selectedRow, 2)->setForeground(Qt::black);
    });

    QObject::connect(deleteButton, &QPushButton::clicked, [&]() {
        int selectedRow = updateTable->currentRow();
        if (selectedRow >= 0) {
            QString itemName = updateTable->item(selectedRow, 0)->text();
            QSqlQuery delQuery;
            delQuery.prepare("DELETE FROM Items WHERE item_name = ?");
            delQuery.addBindValue(itemName);
            if (!delQuery.exec()) {
                QMessageBox::critical(&updateDialog, "Database Error", "Failed to delete item: " + delQuery.lastError().text());
                return;
            }
            updateTable->removeRow(selectedRow);
        } else {
            QMessageBox::warning(&updateDialog, "Warning", "No row selected!");
        }
    });

    QObject::connect(saveButton, &QPushButton::clicked, [&]() {
        QSqlQuery updateQuery;
        for (int i = 0; i < updateTable->rowCount(); i++) {
            updateQuery.prepare("UPDATE Items SET price = ?, stock_quantity = ? WHERE item_name = ?");
            updateQuery.addBindValue(updateTable->item(i, 1)->text().toDouble());
            updateQuery.addBindValue(updateTable->item(i, 2)->text().toInt());
            updateQuery.addBindValue(updateTable->item(i, 0)->text());
            if (!updateQuery.exec()) {
                QMessageBox::critical(&updateDialog, "Database Error", "Failed to update item: " + updateQuery.lastError().text());
            }
        }
        updateDialog.accept();
        loadInventoryData();
    });

    layout->addWidget(updateTable);
    layout->addWidget(editButton);
    layout->addWidget(deleteButton);
    layout->addWidget(saveButton);
    updateDialog.setLayout(layout);

    updateDialog.exec();

    // Reset the blinking state to ensure proper colors
    blinkState = false;
    updateBlinkEffect();
}

// The cell activated function is still defined but not connected anymore
void manage_inventory::on_tableWidget_cellActivated(int row, int column)
{
    if (ui->tableWidget->item(row, column)) {
        QString value = ui->tableWidget->item(row, column)->text();

        if (column == 2 && value.contains(" - Low stock please update")) {
            value = value.split(" - ").first();
        }

        // We don't show this message anymore
        // QMessageBox::information(this, "Cell Activated", "You clicked on: " + value);
    }
}

void manage_inventory::applyFilter()
{
    QString queryStr = "SELECT item_name, price, stock_quantity FROM Items";

    int option = filterComboBox->currentIndex();
    switch (option) {
    case 0:
        queryStr += " ORDER BY item_name ASC";
        break;
    case 1:
        queryStr += " ORDER BY item_name DESC";
        break;
    case 2:
        queryStr += " ORDER BY stock_quantity ASC";
        break;
    case 3:
        queryStr += " ORDER BY stock_quantity DESC";
        break;
    }

    QSqlQuery query(queryStr);
    if (!query.exec()) {
        QMessageBox::critical(this, "Database Error", "Failed to fetch sorted inventory data: " + query.lastError().text());
        return;
    }

    ui->tableWidget->setRowCount(0); // Clear table before loading sorted data

    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(QString::number(query.value(1).toDouble())));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(query.value(2).toInt())));

        // Set default colors for all items after filtering
        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem* item = ui->tableWidget->item(row, col);
            if (item) {
                item->setForeground(Qt::black);
                item->setBackground(Qt::white);
            }
        }

        row++;
    }
}

void manage_inventory::resizeEvent(QResizeEvent *event)
{
    QDialog::resizeEvent(event);
    ui->tableWidget->setGeometry(20, 70, width() * 0.85, height() * 0.75);

    titleLabel->setGeometry(20, 20, 120, 25);
    filterLabel->setGeometry(150, 20, 50, 25);
    filterComboBox->setGeometry(205, 20, 200, 25);
    filterButton->setGeometry(415, 20, 100, 25);
}

void manage_inventory::on_pushButton_2_clicked()
{
    this->close();
}
