#ifndef ADD_UPDATE_H
#define ADD_UPDATE_H

#include <QDialog>

namespace Ui {
class add_update;
}

class add_update : public QDialog
{
    Q_OBJECT

public:
    explicit add_update(QWidget *parent = nullptr);
    ~add_update();

private:
    Ui::add_update *ui;
};

#endif // ADD_UPDATE_H
