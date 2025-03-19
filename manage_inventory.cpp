#include "manage_inventory.h"
#include "ui_manage_inventory.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>
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

    filterLabel = new QLabel("Sort by:", this);
    filterLabel->setGeometry(150, 20, 50, 25);

    filterComboBox = new QComboBox(this);
    filterComboBox->setGeometry(205, 20, 200, 25);
    filterComboBox->addItem("Item Name (Ascending)");
    filterComboBox->addItem("Item Name (Descending)");
    filterComboBox->addItem("Stock (Ascending)");
    filterComboBox->addItem("Stock (Descending)");

    filterButton = new QPushButton("Apply Filter", this);
    filterButton->setGeometry(415, 20, 100, 25);
    connect(filterButton, &QPushButton::clicked, this, &manage_inventory::applyFilter);

    connect(ui->tableWidget, &QTableWidget::cellActivated, this, &manage_inventory::on_tableWidget_cellActivated);

    blinkTimer = new QTimer(this);
    connect(blinkTimer, &QTimer::timeout, this, &manage_inventory::updateBlinkEffect);
    blinkTimer->start(1000);
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
                            item->setForeground(Qt::black);


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

void manage_inventory::on_pushButton_clicked()
{
    bool ok;
    QString itemName = QInputDialog::getText(this, "Add Item", "Enter item name:", QLineEdit::Normal, "", &ok);
    if (!ok || itemName.isEmpty()) return;

    QString itemPrice = QInputDialog::getText(this, "Add Item", "Enter item price:", QLineEdit::Normal, "", &ok);
    if (!ok || itemPrice.isEmpty()) return;

    QString itemStock = QInputDialog::getText(this, "Add Item", "Enter item stock:", QLineEdit::Normal, "15", &ok);
    if (!ok) return;
    if (itemStock.isEmpty()) itemStock = "15";

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
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(itemStock));

    QMessageBox::information(this, "Success", "Item added successfully!");
}

void manage_inventory::on_pushButton_3_clicked()
{
    QDialog updateDialog(this);
    updateDialog.setWindowTitle("Update Items");
    updateDialog.resize(550, 450);

    QVBoxLayout *layout = new QVBoxLayout(&updateDialog);

    QTableWidget *updateTable = new QTableWidget(&updateDialog);
    updateTable->setColumnCount(3);
    updateTable->setHorizontalHeaderLabels({"Item Name", "Price", "Stock"});
    updateTable->setEditTriggers(QAbstractItemView::AllEditTriggers);


    updateTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    updateTable->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Interactive);
    updateTable->setColumnWidth(2, 200); // Set a good width for the stock column

    int rows = ui->tableWidget->rowCount();
    updateTable->setRowCount(rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < 3; j++) {
            if (ui->tableWidget->item(i, j)) {
                QString cellText = ui->tableWidget->item(i, j)->text();

                if (j == 2 && cellText.contains(" - Low stock please update")) {
                    cellText = cellText.split(" - ").first();
                }

                updateTable->setItem(i, j, new QTableWidgetItem(cellText));
            } else if (j == 2) {
                updateTable->setItem(i, j, new QTableWidgetItem("15"));
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

    QPushButton *saveButton = new QPushButton("Save Changes", &updateDialog);
    QObject::connect(saveButton, &QPushButton::clicked, [&]() {
        int newRowCount = updateTable->rowCount();
        ui->tableWidget->setRowCount(newRowCount);
        for (int i = 0; i < newRowCount; i++) {
            for (int j = 0; j < 3; j++) {
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


        if (column == 2 && value.contains(" - Low stock please update")) {
            value = value.split(" - ").first();
        }

        QMessageBox::information(this, "Cell Activated", "You clicked on: " + value);
    }
}

void manage_inventory::applyFilter()
{
    int option = filterComboBox->currentIndex();

    QList<QStringList> items;
    for (int row = 0; row < ui->tableWidget->rowCount(); ++row) {
        QStringList rowItems;
        bool rowHasData = false;

        for (int col = 0; col < ui->tableWidget->columnCount(); ++col) {
            QTableWidgetItem *item = ui->tableWidget->item(row, col);
            if (item) {
                QString cellText = item->text();

                if (col == 2 && cellText.contains(" - Low stock please update")) {
                    cellText = cellText.split(" - ").first();
                }

                rowItems << cellText;
                rowHasData = true;
            } else {
                rowItems << "";
            }
        }

        if (rowHasData) {
            items << rowItems;
        }
    }

    switch (option) {
    case 0:
        std::sort(items.begin(), items.end(), [](const QStringList &a, const QStringList &b) {
            return a[0].toLower() < b[0].toLower();
        });
        break;
    case 1:
        std::sort(items.begin(), items.end(), [](const QStringList &a, const QStringList &b) {
            return a[0].toLower() > b[0].toLower();
        });
        break;
    case 2:
        std::sort(items.begin(), items.end(), [](const QStringList &a, const QStringList &b) {
            return a[2].toInt() < b[2].toInt();
        });
        break;
    case 3:
        std::sort(items.begin(), items.end(), [](const QStringList &a, const QStringList &b) {
            return a[2].toInt() > b[2].toInt();
        });
        break;
    }

    ui->tableWidget->setRowCount(items.size());
    for (int row = 0; row < items.size(); ++row) {
        for (int col = 0; col < items[row].size(); ++col) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(items[row][col]));
        }
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
