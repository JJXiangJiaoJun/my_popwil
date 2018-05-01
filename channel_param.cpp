#include "channel_param.h"
#include "ui_channel_param.h"

channel_param::channel_param(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::channel_param)
{
    ui->setupUi(this);
}

channel_param::~channel_param()
{
    delete ui;
}
