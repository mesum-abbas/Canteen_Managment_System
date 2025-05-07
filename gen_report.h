#ifndef GEN_REPORT_H
#define GEN_REPORT_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class gen_report;
}

class gen_report : public QDialog
{
    Q_OBJECT

public:
    explicit gen_report(QWidget *parent = nullptr);
    ~gen_report();

private slots:
    void on_pushButton_2_clicked();
    void on_pushButton_clicked();

private:
    Ui::gen_report *ui;
    QSqlDatabase db;

protected:
    void closeEvent(QCloseEvent *event) override;
};

#endif // GEN_REPORT_H

