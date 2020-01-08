#ifndef GLOBAL_SETTING_H
#define GLOBAL_SETTING_H

#include <QtGlobal>

static const qint32 MAX_POINT=200000;
#include <cmath>
typedef double ChartDataType;
typedef quint32 FrameLengthType;
typedef qint16  FrameFuncType;
typedef double  PosPeakValueType;
typedef double AccPeakValueType;


struct RunningDataStruct
{
    qint32 dataCnt;             //保存点数
    double totalRunningTime;    //总运行时间
    double samplePeroid;        //采样周期 unit:s
    double RefBuffer[MAX_POINT];//保存原始值
    double CurBuffer[MAX_POINT];//保存实际值

    RunningDataStruct(double _samplePeroid = 0.0001)
    {
        Clear();
        samplePeroid = _samplePeroid;
    }

    void Clear()
    {
        dataCnt = 0;
        totalRunningTime = 0.0;
    }

    double GetCur(qint32 idx)
    {
        if(idx>=dataCnt)
            return -1.0;
        return CurBuffer[idx];
    }

    double GetRef(qint32 idx)
    {
        if(idx>=dataCnt)
            return -1.0;
        return RefBuffer[idx];
    }

    void push(double refData,double curData)
    {
        if(dataCnt >= MAX_POINT)
            dataCnt *= 0.8;
        RefBuffer[dataCnt] = refData;
        CurBuffer[dataCnt] = curData;
        dataCnt++;
    }

};

struct RefDataStruct
{
    qint32 dataCnt;             //采样点数
    double samplePeroid;        //采样周期 unit:s
    double buffer[MAX_POINT];   //数据保存buffer
    RefDataStruct()
    {
        dataCnt=0;
        for(int i=0;i<1000;i++)
            buffer[i] = rand()*5;
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
    int repeatCnt;          //重复次数
    SineWaveParamStruct(){}
    SineWaveParamStruct(double _mid,double _amplitude,double _frequency,double _samplePeriod,int _repeatCnt):
        mid(_mid),amplitude(_amplitude),frequency(_frequency),samplePeriod(_samplePeriod),repeatCnt(_repeatCnt)
    {

    }
};

struct PIDParamStruct{
    double P;
    double I;
    double D;
    PIDParamStruct(){
        P=0.0;
        I=0.0;
        D=0.0;
    }
    PIDParamStruct(double _p,double _i,double _d):
        P(_p),I(_i),D(_d)
    {

    }
};


struct DataPacket
{
    ChartDataType elapsedTime;
    ChartDataType series0;
    ChartDataType series1;
};

struct TVCParamStruct{
    double fs,fv,fa;//前馈
    double bs,bv,ba;//反馈
};

struct SystemInfoStruct
{
    double controlInterval;//控制周期 unit:s
    double samplePeriod;// unit:s
    double drawInterval;//绘图周期 unit:s
    double maxOutU;//最大输出限幅
    double maxAbsoluteForce;//最大推力
    double maxAbsolutePosition;//最大位移
    double maxAbsoluteVel;//最大速度
    double maxAbsoluteAcc;//最大加速度
    double maxLoadWeight;//最大负载质量

    SystemInfoStruct()
    {
        controlInterval = 0.0001;
        samplePeriod = 0.001;
        drawInterval = 100;
        maxOutU = 5;
        maxAbsoluteForce = 10;
        maxAbsolutePosition = 10;
        maxAbsoluteVel = 10;
        maxAbsoluteAcc = 2;
        maxLoadWeight = 2;
    }
};

enum ChartDisplayTypeEnum
{
    PlotPos = 0,
    PlotVel = 1,
    PlotAcc = 2,
    PlotF   = 3,
};

enum WaveFormEnum
{
    SineWave = 0,
    RandomWave = 1,
    SineSweepWave = 2,
    TriangleWave = 3,
    EarthquakeWave = 4,
};

enum ControlMethodEnum
{
    PID = 0,
    TVC = 1,
    ThreePID = 2,
    IterativeControl = 3,
};

enum ControlVariableEnum
{
    Pos = 0,
    F   = 1,
};

enum CommandEnum
{
    Start = 0,
    Stop  = 1,
    Reset = 2,
    Upload = 3,
};

enum EchoMessageEnum
{
    ReceiveRefData = 0,
    ExperimentStart = 1,
    ExperimentStop = 2,
};

enum ErrorMessageEnum
{
    RefDataERR = 0,
    ControlParamERR = 1,
};

#endif // GLOBAL_SETTING_H
