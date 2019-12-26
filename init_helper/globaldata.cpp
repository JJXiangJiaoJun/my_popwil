#include <QByteArray>

#include "globaldata.h"
#include "tcpserver.h"

SystemInfoStruct      g_SystemInfo;
PIDParamStruct  g_StaticPIDParam;
PIDParamStruct  g_PosPIDParam;
PIDParamStruct  g_AccPIDParam;
RefDataStruct   g_PosRefArray;
RefDataStruct   g_VelRefArray;
RefDataStruct   g_AccRefArray;

ChartData g_PosData;
ChartData g_AccData;
ChartData g_VelData;

QDoubleBufferedQueue<DataPacket> g_buffer;

//位移峰值
PosPeakValueType g_PosPeakValue;
//加速度峰值
AccPeakValueType g_AccPeakValue;

bool g_IsRunning;
int g_ConnectedClientCount=0;
qint32 g_ControlMethod=ControlMethodEnum::PID;
double g_CurPos = 0.0;
double g_CurVel = 0.0;
double g_CurAcc = 0.0;

TcpMsgServer *g_TcpMsgServer;

QByteArray tcp_buffer;

QStringList g_WaveFormStringList({QString("正弦波"),QString("随机波"),
                                  QString("正弦扫频"),QString("三角波"),QString("地震波")});
QStringList g_ControlMethodStringList({QString("PID"),QString("三参量"),
                                       QString("3PID"),QString("迭代学习控制")});

ExperimentParamStruct g_ExperimentParam;

qint32  g_ExperimentID;

qint32 g_DisplayType = ChartDisplayTypeEnum::PlotPos;

void SetGlobalExperimentParam(ExperimentParamStruct *paramStruct)
{
    g_ExperimentParam.amplitude = paramStruct->amplitude;
    g_ExperimentParam.frequency = paramStruct->frequency;
    g_ExperimentParam.waveform  = paramStruct->waveform;
}
