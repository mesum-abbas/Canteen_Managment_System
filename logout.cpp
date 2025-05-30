#include "logout.h"
#include "ui_logout.h"
#include <QApplication>
#include "mainwindow.h"
logout::logout(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::logout)
{
    ui->setupUi(this);

    // Adding hover effect and styling for all QPushButtons with larger font
    QString buttonStyle = "QPushButton { color: black; background-color: rgb(255, 191, 60); border-radius: 10px; font-size: 14pt; font-weight: bold; }"
                          "QPushButton:hover { background-color: rgb(255, 215, 80); }";
    ui->yesButton->setStyleSheet(buttonStyle);
    ui->noButton->setStyleSheet(buttonStyle);

    connect(ui->yesButton, &QPushButton::clicked, this, &logout::on_yesButton_clicked);
    connect(ui->noButton, &QPushButton::clicked, this, &logout::on_noButton_clicked);
}
logout::~logout()
{
    delete ui;
}
void logout::on_yesButton_clicked()
{
    this->close();
    MainWindow* existingMainWindow = nullptr;
    QList<QWidget*> topLevelWidgets = QApplication::topLevelWidgets();
    for (QWidget* widget : topLevelWidgets) {
        if (qobject_cast<MainWindow*>(widget)) {
            existingMainWindow = qobject_cast<MainWindow*>(widget);
            break;
        }
    }
    for (QWidget* widget : topLevelWidgets) {
        if (widget != this && widget != existingMainWindow) {
            widget->close();
        }
    }
    if (existingMainWindow) {
        existingMainWindow->show();
        existingMainWindow->activateWindow();
    } else {
        MainWindow *mainWindow = new MainWindow();
        mainWindow->show();
    }
}
void logout::on_noButton_clicked()
{
    this->close();
}
