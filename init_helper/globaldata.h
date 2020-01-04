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
extern  PIDParamStruct  g_AccPIDParam;  //用于力的PID

extern  RefDataStruct     g_PosRefArray;
extern  RefDataStruct     g_VelRefArray;
extern  RefDataStruct     g_AccRefArray;
extern  RefDataStruct   g_FRefArray;

extern RunningDataStruct g_PosRunningData;
extern RunningDataStruct g_VelRunningData;
extern RunningDataStruct g_AccRunningData;
extern RunningDataStruct g_FRunningData;

extern ChartData g_PosData;
extern ChartData g_AccData;
extern ChartData g_VelData;
extern ChartData g_FData;


extern QStringList g_WaveFormStringList;
extern QStringList g_ControlMethodStringList;
extern QStringList g_CommandStringList;
extern QStringList g_EchoStringList;
extern QStringList g_ControlVariableStringList;
extern QDoubleBufferedQueue<DataPacket> g_buffer;
//位移峰值
extern PosPeakValueType g_PosPeakValue;
//加速度峰值
extern AccPeakValueType g_AccPeakValue;

extern bool g_IsRunning;
extern int g_ConnectedClientCount;
extern qint16 g_ControlMethod;
extern qint16 g_ControlVariable;
extern double g_CurPos;
extern double g_CurVel;
extern double g_CurAcc;
extern double g_CurF;
extern double g_StaticVoltage;

extern TcpMsgServer *g_TcpMsgServer;

extern QByteArray tcp_buffer;

extern ExperimentParamStruct g_ExperimentParam;

extern qint32  g_ExperimentID;

extern qint32 g_DisplayType;

void SetGlobalExperimentParam(ExperimentParamStruct *paramStruct);



#endif // GLOBALDATA_H
