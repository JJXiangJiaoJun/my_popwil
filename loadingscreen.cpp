#include "loadingscreen.h"
#include <QPixmap>
#include <QLabel>
loadingscreen::loadingscreen(QWidget *parent):QSplashScreen(parent)
{
    this->setPixmap(QPixmap(":/Image/Ressources/backphoto.png"));
    this->setFont(QFont(tr("宋体"),65,QFont::Bold));
    this->showMessage(QObject::tr("popwil振动平台"),Qt::AlignCenter,Qt::white);

    QLabel *title = new QLabel(this);
    title->setFont(QFont(tr("宋体"),15,QFont::Bold));
    title->setText(QString("正在初始化设置，启动服务器......."));

    //this->showMessage(QObject::tr("振动平台上位机软件"),Qt::AlignCenter,Qt::black);
}
