#include "loginscreen.h"
#include "ui_loginscreen.h"

loginscreen::loginscreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::loginscreen)
{
    ui->setupUi(this);

}

loginscreen::~loginscreen()
{
    delete ui;
}
