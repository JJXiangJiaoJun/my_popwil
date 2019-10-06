#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QtGlobal>

#include "chartdata.h"
#include "global_setting.h"
//#include "tcpserver.h"
#include "global_setting.h"


class TcpMsgServer;

extern ChartData g_PosData;
extern ChartData g_AccData;

//位移峰值
extern PosPeakValueType g_PosPeakValue;
//加速度峰值
extern AccPeakValueType g_AccPeakValue;

extern bool g_IsRunning;

extern TcpMsgServer *g_TcpMsgServer;

extern QByteArray tcp_buffer;

extern ExperimentParamStruct g_ExperimentParam;

extern qint32  g_ExperimentID;
void SetGlobalExperimentParam(ExperimentParamStruct *paramStruct);

///**
// * @brief The Global class
// * 所有的全局成员变量均在此设置
// */

//class GlobalData
//{
//public:
//    static ChartData g_PosData;
//    static ChartData g_AccData;

//    //位移峰值
//    static PosPeakValueType g_PosPeakValue;
//    //加速度峰值
//    static AccPeakValueType g_AccPeakValue;

//    static bool g_IsRunning;

//    static TcpMsgServer *g_TcpMsgServer;
//};

#endif // GLOBALDATA_H
