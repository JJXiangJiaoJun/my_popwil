#ifndef SHAKE_TABLE_PARA_H
#define SHAKE_TABLE_PARA_H

#include <QDialog>

namespace Ui {
class shake_table_para;
}

class shake_table_para : public QDialog
{
    Q_OBJECT

public:
    explicit shake_table_para(QWidget *parent = 0);
    ~shake_table_para();

private:
    Ui::shake_table_para *ui;
};

#endif // SHAKE_TABLE_PARA_H
