#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QtGlobal>

#include "chartdata.h"
#include "global_setting.h"



class TcpMsgServer;

extern  SystemInfo        g_SystemInfo;
extern  PIDParamStruct    g_PIDParam;
extern  RefDataStruct     g_PosRefArray;
extern  RefDataStruct     g_VelRefArray;
extern  RefDataStruct     g_AccRefArray;
extern ChartData g_PosData;
extern ChartData g_AccData;
extern ChartData g_VelData;
//位移峰值
extern PosPeakValueType g_PosPeakValue;
//加速度峰值
extern AccPeakValueType g_AccPeakValue;

extern bool g_IsRunning;

extern TcpMsgServer *g_TcpMsgServer;

extern QByteArray tcp_buffer;

extern ExperimentParamStruct g_ExperimentParam;

extern qint32  g_ExperimentID;

extern qint32 g_DisplayType;

void SetGlobalExperimentParam(ExperimentParamStruct *paramStruct);



#endif // GLOBALDATA_H
