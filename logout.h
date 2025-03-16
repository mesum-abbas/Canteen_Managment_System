#ifndef LOGOUT_H
#define LOGOUT_H

#include <QDialog>

namespace Ui {
class logout;
}

class logout : public QDialog
{
    Q_OBJECT

public:
    explicit logout(QWidget *parent = nullptr);
    ~logout();

private:
    Ui::logout *ui;
private slots:
    void on_yesButton_clicked();
    void on_noButton_clicked();
};


#endif // LOGOUT_H
