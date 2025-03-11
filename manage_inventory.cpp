#include "manage_inventory.h"
#include "ui_manage_inventory.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

manage_inventory::manage_inventory(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::manage_inventory)
{
    ui->setupUi(this);


    ui->tableWidget->setColumnCount(2);
    QStringList headers = {"Item Name", "Price"};
    ui->tableWidget->setHorizontalHeaderLabels(headers);


    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->tableWidget, &QTableWidget::cellActivated, this, &manage_inventory::on_tableWidget_cellActivated);
}

manage_inventory::~manage_inventory()
{
    delete ui;
}

void manage_inventory::on_pushButton_clicked()
{
    bool ok;
    QString itemName = QInputDialog::getText(this, "Add Item", "Enter item name:", QLineEdit::Normal, "", &ok);
    if (!ok || itemName.isEmpty()) return;

    QString itemPrice = QInputDialog::getText(this, "Add Item", "Enter item price:", QLineEdit::Normal, "", &ok);
    if (!ok || itemPrice.isEmpty()) return;


    int rowCount = ui->tableWidget->rowCount();
    int emptyRow = -1;

    for (int i = 0; i < rowCount; i++) {
        if (!ui->tableWidget->item(i, 0) || ui->tableWidget->item(i, 0)->text().isEmpty()) {
            emptyRow = i;
            break;
        }
    }


    int row = (emptyRow == -1) ? rowCount : emptyRow;
    if (emptyRow == -1) {
        ui->tableWidget->insertRow(row);
    }

    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(itemName));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(itemPrice));

    QMessageBox::information(this, "Success", "Item added successfully!");
}

void manage_inventory::on_pushButton_3_clicked()
{
    QDialog updateDialog(this);
    updateDialog.setWindowTitle("Update Items");
    updateDialog.resize(450, 350);

    QVBoxLayout *layout = new QVBoxLayout(&updateDialog);

    QTableWidget *updateTable = new QTableWidget(&updateDialog);
    updateTable->setColumnCount(2);
    updateTable->setHorizontalHeaderLabels({"Item Name", "Price"});
    updateTable->setEditTriggers(QAbstractItemView::AllEditTriggers);

    int rows = ui->tableWidget->rowCount();
    updateTable->setRowCount(rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 2; j++) {
            if (ui->tableWidget->item(i, j)) {
                updateTable->setItem(i, j, new QTableWidgetItem(ui->tableWidget->item(i, j)->text()));
            }
        }
    }

    QPushButton *deleteButton = new QPushButton("Delete Selected Row", &updateDialog);
    QObject::connect(deleteButton, &QPushButton::clicked, [&]() {
        int selectedRow = updateTable->currentRow();
        if (selectedRow >= 0) {
            updateTable->removeRow(selectedRow);
        } else {
            QMessageBox::warning(&updateDialog, "Warning", "No row selected!");
        }
    });

    // Save button
    QPushButton *saveButton = new QPushButton("Save Changes", &updateDialog);
    QObject::connect(saveButton, &QPushButton::clicked, [&]() {
        int newRowCount = updateTable->rowCount();
        ui->tableWidget->setRowCount(newRowCount);
        for (int i = 0; i < newRowCount; i++) {
            for (int j = 0; j < 2; j++) {
                if (updateTable->item(i, j)) {
                    ui->tableWidget->setItem(i, j, new QTableWidgetItem(updateTable->item(i, j)->text()));
                }
            }
        }
        updateDialog.accept();
    });

    layout->addWidget(updateTable);
    layout->addWidget(deleteButton);
    layout->addWidget(saveButton);
    updateDialog.setLayout(layout);

    updateDialog.exec();
}

void manage_inventory::on_tableWidget_cellActivated(int row, int column)
{
    if (ui->tableWidget->item(row, column)) {
        QString value = ui->tableWidget->item(row, column)->text();
        QMessageBox::information(this, "Cell Activated", "You clicked on: " + value);
    }
}
