#include "shake_table_para.h"
#include "ui_shake_table_para.h"

shake_table_para::shake_table_para(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::shake_table_para)
{
    ui->setupUi(this);
}

shake_table_para::~shake_table_para()
{
    delete ui;
}
