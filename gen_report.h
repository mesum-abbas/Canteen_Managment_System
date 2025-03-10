#ifndef GEN_REPORT_H
#define GEN_REPORT_H

#include <QDialog>

namespace Ui {
class gen_report;
}

class gen_report : public QDialog
{
    Q_OBJECT

public:
    explicit gen_report(QWidget *parent = nullptr);
    ~gen_report();

private:
    Ui::gen_report *ui;
};

#endif // GEN_REPORT_H
