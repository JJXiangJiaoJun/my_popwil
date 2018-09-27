#include "restricted_para.h"
#include "ui_restricted_para.h"

restricted_para::restricted_para(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::restricted_para)
{
    ui->setupUi(this);
}

restricted_para::~restricted_para()
{
    delete ui;
}
