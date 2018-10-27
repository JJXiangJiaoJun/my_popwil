#include "mainwindow.h"
#include <QApplication>
#include "loadingscreen.h"
#include<QTextCodec>
#include<QHostInfo>
#include "myhelper.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    //设置样式表
    myHelper::setStyle("pop_wil");

    //MainWindow w;
    //BaseWindow test;
    MyMainWindow w;
    //loadingscreen *load = new loadingscreen;
   // load->show();
    //Sleep(2000);
    w.show();
   // test.show();
   //oad->finish(&w);
   // delete load;
    return a.exec();
}
