#ifndef CONTROL_PARAM_H
#define CONTROL_PARAM_H

#include <QWidget>

namespace Ui {
class control_param;
}

class control_param : public QWidget
{
    Q_OBJECT

public:
    explicit control_param(QWidget *parent = 0);
    ~control_param();

private:
    Ui::control_param *ui;
};

#endif // CONTROL_PARAM_H
