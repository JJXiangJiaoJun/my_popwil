#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "loginscreen.h"
#include "control_param.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Start_btn_clicked()
{
    qDebug()<<"clicked";
}


//点击菜单栏用户登录按钮
void MainWindow::on_User_1_triggered()
{
    loginscreen *log = new loginscreen ;
    log->show();
}


//点击菜单栏控制参数设置
void MainWindow::on_Setting_control_para_triggered()
{
    control_param *control_window = new control_param;
    control_window->show();
}
