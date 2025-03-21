#include <QApplication>
#include "mainwindow.h"
#include "database.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    DatabaseManager dbManager;
    if (!dbManager.connectDatabase()) {
        return -1;  // Exit if database connection fails
    }

    MainWindow w;
    w.show();
    return app.exec();
}
