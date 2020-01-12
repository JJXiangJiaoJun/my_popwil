#include <QByteArray>

#include "globaldata.h"
#include "tcpserver.h"

SystemInfoStruct      g_SystemInfo;
PIDParamStruct  g_StaticPIDParam;
PIDParamStruct  g_PosPIDParam;
PIDParamStruct  g_AccPIDParam;
//参考波形数据
RefDataStruct   g_PosRefArray;
RefDataStruct   g_VelRefArray;
RefDataStruct   g_AccRefArray;
RefDataStruct   g_FRefArray;
//试验实时数据
RunningDataStruct g_PosRunningData;
RunningDataStruct g_VelRunningData;
RunningDataStruct g_AccRunningData;
RunningDataStruct g_FRunningData;
//绘图数据
ChartData g_PosData;
ChartData g_AccData;
ChartData g_VelData;
ChartData g_FData;


QDoubleBufferedQueue<DataPacket> g_buffer;

//位移峰值
PosPeakValueType g_PosPeakValue;
//加速度峰值
AccPeakValueType g_AccPeakValue;

bool g_IsRunning= false;
bool g_SendRef = false;
int g_ConnectedClientCount=0;
qint16 g_ControlMethod=ControlMethodEnum::PID;
qint16 g_ControlVariable = ControlVariableEnum::Pos;

double g_CurPos = 0.0;
double g_CurVel = 0.0;
double g_CurAcc = 0.0;
double g_CurF = 0.0;
double g_StaticVoltage = 0.0;

TcpMsgServer *g_TcpMsgServer;

QByteArray tcp_buffer;

QStringList g_WaveFormStringList({QString("正弦波"),QString("随机波"),
                                  QString("正弦扫频"),QString("三角波"),QString("地震波")});
QStringList g_ControlMethodStringList({QString("PID"),QString("三参量"),
                                       QString("3PID"),QString("迭代学习控制")});
QStringList g_CommandStringList({QString("启动"),QString("停止"),QString("复位零点"),QString("请求上传数据")});

QStringList g_EchoStringList({QString("接收到参考波形"),QString("试验开始"),QString("试验停止")});

QStringList g_ControlVariableStringList({QString("位移控制"),QString("力控制")});


ExperimentParamStruct g_ExperimentParam;

qint32  g_ExperimentID;

qint32 g_DisplayType = ChartDisplayTypeEnum::PlotPos;

void SetGlobalExperimentParam(ExperimentParamStruct *paramStruct)
{
    g_ExperimentParam.amplitude = paramStruct->amplitude;
    g_ExperimentParam.frequency = paramStruct->frequency;
    g_ExperimentParam.waveform  = paramStruct->waveform;
}
