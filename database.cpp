#include "database.h"

DatabaseManager::DatabaseManager(QObject *parent) : QObject(parent) {}

DatabaseManager::~DatabaseManager() {
    if (db.isOpen()) {
        db.close();
        qDebug() << "Database closed.";
    }
}

bool DatabaseManager::connectDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("inventory.db");

    if (!db.open()) {
        qDebug() << "Error: Could not open database!";
        return false;
    }

    qDebug() << "Database connected successfully.";
    return true;
}

QSqlDatabase DatabaseManager::getDatabase() {
    return db;
}
