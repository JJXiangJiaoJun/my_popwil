#ifndef SET_MEASUREMENT_UNIT_H
#define SET_MEASUREMENT_UNIT_H

#include <QDialog>
#include <QTableWidget>

namespace Ui {
class set_measurement_unit;
}

class set_measurement_unit : public QDialog
{
    Q_OBJECT

public:
    explicit set_measurement_unit(QWidget *parent = 0);
    ~set_measurement_unit();

private:
    Ui::set_measurement_unit *ui;
    QTableWidget *change_unit;

};

#endif // SET_MEASUREMENT_UNIT_H
