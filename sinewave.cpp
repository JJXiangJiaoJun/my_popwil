#include "sinewave.h"
#include "ui_sinewave.h"
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
#include <QLineEdit>
sinewave::sinewave(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sinewave)
{
    ui->setupUi(this);

    /*************************设置样式表************************************/
    setStyleSheet("QDialog{border-width:1px;border-color:rgb(48, 104, 151);\
                  border-style: outset;background-color: rgb(132, 171, 208);\
                  background: transparent;border-image: url(:/Image/Ressources/MainWindow.png);}\
                  QLineEdit,QLabel{font-family:\"Times New Roman\";font-size:25px;\
                  } ");

}

sinewave::~sinewave()
{
    delete ui;
}


//按下产生波形的按钮
void sinewave::on_generate_wave_clicked()
{
    //把编辑的正弦波参数读入到公有属性
    ampilitude_text=ui->amplitude->text();
    frequency_text=ui->frequency->text();
    time_text=ui->time->text();
    phase_text=ui->phase->text();
    samplerate_text =ui->samplerate->text();

    qDebug()<<ampilitude_text<<frequency_text<<time_text<<phase_text<<samplerate_text;

    //发送信号
    emit sinewave_para(ampilitude_text,frequency_text,time_text,phase_text,samplerate_text);

    QMessageBox msg(QMessageBox::NoIcon,"提示","成功生成正弦波形");
    msg.setIconPixmap(QPixmap(":/my_beautiful_icon/Ressources/beautiful/Task Manager alt 2.png"));
    //msg.setText("已经成功生成波形");
    msg.exec();
    this->close();
}


void sinewave::on_cancel_clicked()
{
    this->close();
}
