#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include"admin_authentication.h"
#include"user_authentication.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::MainWindow *ui;
    admin_authentication *ptradminauthentication;
    user_authentication *ptruserauthentication;

};
#endif // MAINWINDOW_H
