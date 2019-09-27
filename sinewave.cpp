#include "sinewave.h"
#include "ui_sinewave.h"


#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
#include <QLineEdit>
#include <QtGlobal>
#include <QDoubleValidator>


#include "protocol.h"
#include "tcpserver.h"

extern TcpMsgServer * g_TcpMsgServer;


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

   //设置输入框的格式


    Amplitude_LineEdit = ui->Amplitude_LineEdit;
    Frequency_LineEdit = ui->Frequency_LineEdit;
    QDoubleValidator *pAmplitude_Validator = new QDoubleValidator(-180.0,180.0,4,Amplitude_LineEdit);
    QDoubleValidator *pFrequency_Validator = new QDoubleValidator(-180.0,180.0,4,Frequency_LineEdit);

    Amplitude_LineEdit->setPlaceholderText("请输入正弦波幅值");
    pAmplitude_Validator->setNotation(QDoubleValidator::StandardNotation);
    Amplitude_LineEdit->setValidator(pAmplitude_Validator);

    Frequency_LineEdit->setPlaceholderText("请输入正弦波频率");
    pFrequency_Validator->setNotation(QDoubleValidator::StandardNotation);
    Frequency_LineEdit->setValidator(pFrequency_Validator);


}

sinewave::~sinewave()
{
    delete ui;
}


//按下产生波形的按钮
void sinewave::on_generate_wave_clicked()
{

    double amplitiude;
    double frequency;
    qint16 waveform = 1;
    qint32 msg_len = sizeof(ExperimentParmStruct);
    ExperimentParmStruct *param_prt = new ExperimentParmStruct;

    //把编辑的正弦波参数读入到公有属性
    ampilitude_text=ui->Amplitude_LineEdit->text();
    frequency_text=ui->Frequency_LineEdit->text();
    time_text=ui->time->text();
    phase_text=ui->phase->text();
    samplerate_text =ui->samplerate->text();


    amplitiude = ampilitude_text.toDouble();
    frequency = frequency_text.toDouble();

    //构造数据帧
    param_prt->waveform = waveform;
    param_prt->amplitude = amplitiude;
    param_prt->frequency = frequency;

    qDebug()<<"幅值"<<param_prt->amplitude << "相位" <<param_prt->frequency;

    //发送数据帧
    //GlobalData::g_TcpMsgServer->SendMsgToClient(ProtocolSet::PARAM,(void *) param_prt,msg_len);
    g_TcpMsgServer->SendMsgToClient(ProtocolSet::PARAM,(void *) param_prt,msg_len);

    //删除构造的数据帧
    delete param_prt;





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
