#include "sinewave.h"
#include "ui_sinewave.h"


#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
#include <QLineEdit>
#include <QtGlobal>
#include <QDoubleValidator>
#include <QIntValidator>

#include "protocol.h"
#include "tcpserver.h"
#include "SignalGenerator.h"


sinewave::sinewave(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::sinewave)
{
    ui->setupUi(this);

    /*************************设置样式表************************************/
    setStyleSheet("QDialog{border-width:1px;border-color:rgb(48, 104, 151);\
                  border-style: outset;background-color: rgb(132, 171, 208);\
                  background: transparent;border-image: url(:/Image/Ressources/MainWindow.png);}\
                  QLabel{font-family:\"Times New Roman\";font-size:25px;\
                  } QLineEdit{font-family:\"Times New Roman\";font-size:15px;\
                                  }");

    //设置输入框的格式


    Amplitude_LineEdit = ui->Amplitude_LineEdit;
    Frequency_LineEdit = ui->Frequency_LineEdit;
    Mid_LineEdit = ui->Mid_LineEdit;
    SamplePeriod_LineEdit = ui->SamplePeriod_LineEdit;
    RepeatCount_LineEdit = ui->RepeatCount_LineEdit;


    QDoubleValidator *pAmplitude_Validator = new QDoubleValidator(-10000.0,100000.0,4,Amplitude_LineEdit);
    QDoubleValidator *pFrequency_Validator = new QDoubleValidator(0.0,100000.0,4,Frequency_LineEdit);
    QDoubleValidator *pMid_Validator = new QDoubleValidator(-180.0,180.0,4,Mid_LineEdit);
    QIntValidator    *pSamplePeriod_Validator = new QIntValidator(0,1000,SamplePeriod_LineEdit);
    QIntValidator    *pRpeatCount_Validator = new QIntValidator(0,10000,RepeatCount_LineEdit);


    Amplitude_LineEdit->setPlaceholderText("请输入正弦波幅值");
    pAmplitude_Validator->setNotation(QDoubleValidator::StandardNotation);
    Amplitude_LineEdit->setValidator(pAmplitude_Validator);

    Frequency_LineEdit->setPlaceholderText("请输入正弦波频率");
    pFrequency_Validator->setNotation(QDoubleValidator::StandardNotation);
    Frequency_LineEdit->setValidator(pFrequency_Validator);

    Mid_LineEdit->setPlaceholderText("请输入中心坐标");
    pMid_Validator->setNotation(QDoubleValidator::StandardNotation);
    Mid_LineEdit->setValidator(pMid_Validator);

    SamplePeriod_LineEdit->setPlaceholderText("请输入采样周期(ms)");
    //pSamplePeriod_Validator->setNotation(QIntValidator::);
    SamplePeriod_LineEdit->setValidator(pSamplePeriod_Validator);


    RepeatCount_LineEdit->setPlaceholderText("请输入重复次数");
    //pRpeatCount_Validator->setNotation(QIntValidator::StandardNotation);
    RepeatCount_LineEdit->setValidator(pRpeatCount_Validator);



   Amplitude_LineEdit->insert("1.0");
   Frequency_LineEdit->insert("2.0");
   Mid_LineEdit->insert("0.0");
   SamplePeriod_LineEdit->insert(QString::number(int(g_SystemInfo.samplePeriod*1000)));
   RepeatCount_LineEdit->insert("200");
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
    double mid;
    int    samplePeriod;
    int    repeatCount;

    qint16 waveform = WaveFormEnum::SineWave;
    qint32 msg_len = sizeof(ExperimentParamStruct);
    ExperimentParamStruct *param_prt = new ExperimentParamStruct;

    //把编辑的正弦波参数读入到公有属性
    ampilitude_text=Amplitude_LineEdit->text();
    frequency_text=Frequency_LineEdit->text();
    repeatCount_text=RepeatCount_LineEdit->text();
    mid_text=Mid_LineEdit->text();
    samplePeriod_text =SamplePeriod_LineEdit->text();

    //转换为数字
    amplitiude = ampilitude_text.toDouble();
    frequency = frequency_text.toDouble();
    mid = mid_text.toDouble();
    samplePeriod = samplePeriod_text.toInt();
    repeatCount = repeatCount_text.toInt();

    double samplePeriod_s = double(samplePeriod) / 1000;

    SineWaveParamStruct SineParam(mid,amplitiude,frequency,samplePeriod_s,repeatCount);

    //构造数据帧
    param_prt->waveform = waveform;
    param_prt->amplitude = amplitiude;
    param_prt->frequency = frequency;

    qDebug()<<"幅值"<<param_prt->amplitude << "相位" <<param_prt->frequency;

    SetGlobalExperimentParam(param_prt);
    SignalGenerator::GenerateSineWave(SineParam);


    //删除构造的数据帧
    delete param_prt;





    qDebug()<<ampilitude_text<<frequency_text<<mid_text<<repeatCount_text<<samplePeriod_text;

    //发送信号
    emit SignalChangeExperimentParam();

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
