#include "new_experiment.h"
#include "ui_new_experiment.h"
#include <qmath.h>
#include <cmath>
#include <QTextStream>
#include <QDebug>

const double double_Pi = 3.141592757;

new_experiment::new_experiment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::new_experiment)
{
    ui->setupUi(this);
//    max_time = 1000.0;
//    sample_rate =10000.0;
//    amplitude =1;
//    frequecy = 10.0;
//    phase = 0.0;
//    time = 0.0;
    filename = "sinewave.csv";

    //连接信号槽
    connect(&sinepara,SIGNAL(sinewave_para(QString,QString,QString,QString,QString)),\
            this,SLOT(get_sine_wave_para(QString,QString,QString,QString,QString)));
}

void new_experiment::get_sine_wave_para(QString A, QString f, QString t, QString p, QString samp){
    amplitude =A.toFloat();
    frequecy = f.toFloat();
    time = t.toFloat();
    phase = p.toFloat();
    sample_rate = samp.toFloat();
}

new_experiment::~new_experiment()
{
    delete ui;
}

void new_experiment::on_exit_clicked()
{

}

void new_experiment::generate_sine_wave_data(QString filename,float m_amplitude, float m_frequency, float m_time, float m_phase)
{

    QFile file(filename);
    if(m_time>=max_time){
        m_time = 0.0;
        qDebug()<<"取样点数过多";
    }

    qDebug()<<"开始生成正弦波形";
    qint16 bufferTofill = sample_rate/m_frequency+1;

    float *sinewaveBuffer = new float[bufferTofill];

    float m_deltaTime = (float)1/(float)sample_rate;
    //产生正弦波形
    for(int sample =0;sample<bufferTofill;++sample){
        float value = m_amplitude*sin((double)2*double_Pi*m_frequency*m_time+m_phase);

        sinewaveBuffer[sample] = value;
        m_time+=m_deltaTime;
    }
    //产生的正弦波已经保存到数组中，接下来只要写入文件就好了

    qDebug()<<"开始写入文件"<<file.fileName();
    if(!file.open(QIODevice::WriteOnly)){
        qDebug()<<"无法打开文件";
        return;
    }
    QTextStream out(&file);
    //逐个写入文件
    for(int i=0;i<bufferTofill;i++){
        out<<sinewaveBuffer[i]<<","<<endl;
    }

    qDebug()<<"写入完成";

    integrator Integral;
    Integral.Func_Integral_Trapezoid("intergral_sinewave.csv",bufferTofill,sinewaveBuffer,sample_rate/frequecy);

    delete sinewaveBuffer;
}


//按下正弦试验按钮生成正弦波形
void new_experiment::on_sin_clicked()
{

    // qfloat16 deltaTime = 1/sample_rate;
    //sinepara = new sinewave(this);
    sinepara.exec();
    generate_sine_wave_data(filename,amplitude,frequecy,time,phase);

}


void integrator::Func_Integral_Trapezoid(QString filename, int n, float *data,int interval)
{
    float *inter_data = new float[n+5];
    //梯形公式近似
    for(int i=1;i<n;i++)
    {
        //积分公式，近似梯形积分
        inter_data[i] =inter_data[i-1]+((data[i-1]+data[i])/2)*(2*double_Pi/interval);
    }
     QFile file(filename);
     qDebug()<<"开始写入积分文件"<<file.fileName();
     if(!file.open(QIODevice::WriteOnly)){
         qDebug()<<"无法打开文件";
         return;
     }
     QTextStream out(&file);
     for(int i=1;i<n;i++){
         out<<inter_data[i]<<","<<endl;
     }
     qDebug()<<"写入完成";

     delete inter_data;

}
