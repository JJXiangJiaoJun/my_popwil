#include "SignalGenerator.h"
#define PI acos(-1.0)

SignalGenerator::SignalGenerator()
{

}

void SignalGenerator::GenerateSineWave(SineWaveParamStruct param)
{
    double mid = param.mid;                     //中心坐标
    double amplitude = param.amplitude;         //幅值
    double frequency = param.frequency;         //频率
    double wavePeriod = 1.0/frequency;          //周期
    double samplePeriod = param.samplePeriod;   //采样周期
    double repeatCnt = double(param.repeatCnt);         //重复次数

    //生成参考波形
    g_PosRefArray.dataCnt = 0;g_VelRefArray.dataCnt = 0;g_AccRefArray.dataCnt = 0;
    g_PosRefArray.samplePeroid = samplePeriod;g_VelRefArray.samplePeroid = samplePeriod;g_AccRefArray.samplePeroid = samplePeriod;
    double cur_t = 0.0;                         //当前时间
    while(cur_t<=(double)repeatCnt*wavePeriod)
    {
        g_PosRefArray.buffer[g_PosRefArray.dataCnt++] = amplitude*sin(2*PI*frequency*cur_t) + mid;
        g_VelRefArray.buffer[g_VelRefArray.dataCnt++] = amplitude*2*PI*frequency*cos(2*PI*frequency*cur_t);
        g_AccRefArray.buffer[g_AccRefArray.dataCnt++] = -amplitude*4*PI*PI*frequency*frequency*sin(2*PI*frequency*cur_t)/10000;
        cur_t += samplePeriod;
    }

}


void SignalGenerator::GenerateRandomWave(double amplitude)
{
    g_PosRefArray.dataCnt = 10000; g_PosRefArray.samplePeroid = 0.001;
    for(int i=0;i<10000;i++)
        g_PosRefArray.buffer[i]=amplitude*(random(1000)*1.0/500-1);
}


void SignalGenerator::GenerateTriangleWave(SineWaveParamStruct param)
{

}

void SignalGenerator::GenerateSineSweepWave(QString path)
{

}


void SignalGenerator::GenerateEarthQuakeWave(QString path)
{
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug()<<"地震波文件"+path+"文件打开失败";
        QMessageBox::warning(NULL,"警告","地震波文件打开失败",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
        return;
    }

    int cnt=0;
    QTextStream in(&file);
    while(!in.atEnd())
    {
        cnt++;
        QString line = in.readLine();
        line = line.trimmed();
        line = line.simplified();
        QStringList lines = line.split(" ");
        if(cnt==1)
        {
            g_AccRefArray.samplePeroid = (double)lines[1].toInt()/1000.0;
            continue;
        }
        if(cnt==2)
            continue;
        int i=lines[0].toInt();
        g_AccRefArray.buffer[i] = lines[3].toDouble();
    }
    g_AccRefArray.dataCnt = cnt-2;
}
