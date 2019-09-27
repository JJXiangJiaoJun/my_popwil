#include "globaldata.h"

#include <QByteArray>

#include "tcpserver.h"


//用于给chart_direct绘图的全局变量
//ChartData GlobalData::g_PosData;
//ChartData GlobalData::g_AccData;
//PosPeakValueType GlobalData::g_PosPeakValue;
//AccPeakValueType GlobalData::g_AccPeakValue;
//bool GlobalData::g_IsRunning = false;
//TcpMsgServer * GlobalData::g_TcpMsgServer;

ChartData g_PosData;
ChartData g_AccData;

//位移峰值
PosPeakValueType g_PosPeakValue;
//加速度峰值
AccPeakValueType g_AccPeakValue;

bool g_IsRunning;

TcpMsgServer *g_TcpMsgServer;

QByteArray tcp_buffer;
