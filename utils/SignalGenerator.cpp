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
    double repeatCnt = param.repeatCnt;         //重复次数

    //生成参考波形
    g_PosRefArray.dataCnt = 0;g_VelRefArray.dataCnt = 0;g_AccRefArray.dataCnt = 0;
    g_PosRefArray.samplePeroid = samplePeriod;g_VelRefArray.samplePeroid = samplePeriod;g_AccRefArray.samplePeroid = samplePeriod;
    double cur_t = 0.0;                         //当前时间
    while(cur_t>repeatCnt*wavePeriod)
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
