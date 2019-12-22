#ifndef GLOBAL_SETTING_H
#define GLOBAL_SETTING_H

#include <QtGlobal>

static const qint32 MAX_POINT=200000;

typedef double ChartDataType;
typedef quint16 FrameLengthType;
typedef qint16  FrameFuncType;
typedef double  PosPeakValueType;
typedef double AccPeakValueType;


struct RefDataStruct
{
    qint32 dataCnt;             //采样点数
    double samplePeroid;        //采样周期 unit:s
    double buffer[MAX_POINT];   //数据保存buffer
    RefDataStruct()
    {

    }

};

struct ExperimentParamStruct
{
    qint16 waveform;
    double amplitude;
    double frequency;
    double repeatCnt;
};

struct SineWaveParamStruct
{
    double mid;                //中心坐标
    double amplitude;          //幅值
    double frequency;          //频率
    double samplePeriod;       //采样周期 unit:s
    double repeatCnt;          //重复次数
    SineWaveParamStruct(){}
    SineWaveParamStruct(double _mid,double _amplitude,double _frequency,double _samplePeriod,double _repeatCnt):
        mid(_mid),amplitude(_amplitude),frequency(_frequency),samplePeriod(_samplePeriod),repeatCnt(_repeatCnt)
    {

    }
};

struct PIDParamStruct{
    double P;
    double I;
    double D;
    PIDParamStruct(){}
    PIDParamStruct(double _p,double _i,double _d):
        P(_p),I(_i),D(_d)
    {

    }
};

struct TVCParamStruct{
    double fs,fv,fa;//前馈
    double bs,bv,ba;//反馈
};

struct SystemInfo
{
    double controlInterval;//控制周期 unit:s
    double samplePeriod;// unit:s
    double drawInterval;//绘图周期
    double maxOutU;//最大输出限幅
    double maxAbsoluteForce;//最大推力
    double maxAbsolutePosition;//最大位移
    double maxAbsoluteVel;//最大速度
    double maxAbsoluteAcc;//最大加速度
    double maxLoadWeight;//最大负载质量
};

enum ChartDisplayTypeEnum
{
    PlotPos = 0,
    PlotVel = 1,
    PlotAcc = 2,
};

enum WaveFormEnum
{
    SineWave = 0,
    RandomWave = 1,
    SineSweepWave = 2,
    TriangleWave = 3,
    EarthquakeWave = 4,
};



#endif // GLOBAL_SETTING_H
