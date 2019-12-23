#include <QByteArray>

#include "globaldata.h"
#include "tcpserver.h"

SystemInfo      g_SystemInfo;
PIDParamStruct  g_PIDParam;
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

TcpMsgServer *g_TcpMsgServer;

QByteArray tcp_buffer;


ExperimentParamStruct g_ExperimentParam;

qint32  g_ExperimentID;

qint32 g_DisplayType = ChartDisplayTypeEnum::PlotPos;

void SetGlobalExperimentParam(ExperimentParamStruct *paramStruct)
{
    g_ExperimentParam.amplitude = paramStruct->amplitude;
    g_ExperimentParam.frequency = paramStruct->frequency;
    g_ExperimentParam.waveform  = paramStruct->waveform;
}
