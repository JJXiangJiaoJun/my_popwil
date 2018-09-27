#ifndef RESTRICTED_PARA_H
#define RESTRICTED_PARA_H

#include <QDialog>

namespace Ui {
class restricted_para;
}

class restricted_para : public QDialog
{
    Q_OBJECT

public:
    explicit restricted_para(QWidget *parent = 0);
    ~restricted_para();

private:
    Ui::restricted_para *ui;
};

#endif // RESTRICTED_PARA_H
