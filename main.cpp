#include "mainwindow.h"
#include <QApplication>
#include "loadingscreen.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //loadingscreen *load = new loadingscreen;
   // load->show();
    //Sleep(2000);
    w.show();
   //oad->finish(&w);
   // delete load;
    return a.exec();
}
