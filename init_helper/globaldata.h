#ifndef GLOBALDATA_H
#define GLOBALDATA_H

#include <QtGlobal>
#include <QString>
#include <QStringList>

#include "chartdata.h"
#include "qdoublebufferedqueue.h"
#include "global_setting.h"



class TcpMsgServer;

extern  SystemInfoStruct        g_SystemInfo;
extern  PIDParamStruct  g_StaticPIDParam;
extern  PIDParamStruct  g_PosPIDParam;
extern  PIDParamStruct  g_AccPIDParam;

extern  RefDataStruct     g_PosRefArray;
extern  RefDataStruct     g_VelRefArray;
extern  RefDataStruct     g_AccRefArray;

extern ChartData g_PosData;
extern ChartData g_AccData;
extern ChartData g_VelData;

extern QStringList g_WaveFormStringList;
extern QStringList g_ControlMethodStringList;

extern QDoubleBufferedQueue<DataPacket> g_buffer;
//位移峰值
extern PosPeakValueType g_PosPeakValue;
//加速度峰值
extern AccPeakValueType g_AccPeakValue;

extern bool g_IsRunning;
extern int g_ConnectedClientCount;
extern qint32 g_ControlMethod;
extern double g_CurPos;
extern double g_CurVel;
extern double g_CurAcc;

extern TcpMsgServer *g_TcpMsgServer;

extern QByteArray tcp_buffer;

extern ExperimentParamStruct g_ExperimentParam;

extern qint32  g_ExperimentID;

extern qint32 g_DisplayType;

void SetGlobalExperimentParam(ExperimentParamStruct *paramStruct);



#endif // GLOBALDATA_H
