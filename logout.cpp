#include "logout.h"
#include "ui_logout.h"
#include <QApplication>
#include "mainwindow.h"

logout::logout(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::logout)
{
    ui->setupUi(this);

    // Connect buttons to their respective slots
    connect(ui->yesButton, &QPushButton::clicked, this, &logout::on_yesButton_clicked);
    connect(ui->noButton, &QPushButton::clicked, this, &logout::on_noButton_clicked);
}

logout::~logout()
{
    delete ui;
}

void logout::on_yesButton_clicked()
{
    // Close this dialog first
    this->close();

    // Find existing MainWindow
    MainWindow* existingMainWindow = nullptr;

    // Get all top-level widgets
    QList<QWidget*> topLevelWidgets = QApplication::topLevelWidgets();

    // First find the MainWindow
    for (QWidget* widget : topLevelWidgets) {
        if (qobject_cast<MainWindow*>(widget)) {
            existingMainWindow = qobject_cast<MainWindow*>(widget);
            break;
        }
    }

    // Now close all other windows except MainWindow
    for (QWidget* widget : topLevelWidgets) {
        if (widget != this && widget != existingMainWindow) {
            widget->close();
        }
    }

    // If we found an existing MainWindow, show it; otherwise create a new one
    if (existingMainWindow) {
        existingMainWindow->show();
        existingMainWindow->activateWindow(); // Brings it to the front
    } else {
        // Create and show a new MainWindow if somehow it doesn't exist
        MainWindow *mainWindow = new MainWindow();
        mainWindow->show();
    }
}

void logout::on_noButton_clicked()
{
    // Just close the logout dialog
    this->close();
}
