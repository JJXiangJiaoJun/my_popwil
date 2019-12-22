#include "control_param.h"
#include "ui_control_param.h"


control_param::control_param(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::control_param)
{
    ui->setupUi(this);
}

control_param::~control_param()
{
    delete ui;
}
