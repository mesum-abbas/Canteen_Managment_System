#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

class DatabaseManager : public QObject {
    Q_OBJECT

public:
    explicit DatabaseManager(QObject *parent = nullptr);
    ~DatabaseManager();

    bool connectDatabase();
    QSqlDatabase getDatabase();

private:
    QSqlDatabase db;
};

#endif // DATABASE_H
