#ifndef STOCK_ALERT_H
#define STOCK_ALERT_H

#include <QDialog>

namespace Ui {
class stock_alert;
}

class stock_alert : public QDialog
{
    Q_OBJECT

public:
    explicit stock_alert(QWidget *parent = nullptr);
    ~stock_alert();

private:
    Ui::stock_alert *ui;
};

#endif // STOCK_ALERT_H
