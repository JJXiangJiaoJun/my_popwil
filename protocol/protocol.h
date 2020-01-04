#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QtGlobal>
#include <QObject>
#include <QWidget>

#include "globaldata.h"
#include "global_setting.h"
//////////////////////////////////////////////////////////////////////////////
/// \brief The ProtocolSet class
///构造数据报的协议
///

#define PROTOCOL_HEAD 0xAAAA
#define PACKAGE_MINIMA_SIZE sizeof(FrameLengthType)+sizeof(FrameFuncType)



class ProtocolSet
{
public:
    ProtocolSet();

    static FrameLengthType FrameLegthLen;
    static FrameFuncType FrameFuncLen;
    static QString ConfigFile;          //配置文件路径
    static QString SendFileName;        //发送配置文件名
    static QString DeviceFileName;      //模拟设备数据文件名

    //TCP服务器配置参数
    static bool HexSendTcpServerAbstract;       //16进制发送
    static bool HexReceiveTcpServerAbstract;    //16进制接收
    static bool AsciiTcpServerAbstract;         //ASCII模式
    static bool DebugTcpServerAbstract;         //启用数据调试
    static bool AutoSendTcpServerAbstract;      //自动发送数据
    static int IntervalTcpServerAbstract;       //发送数据间隔
    static int TcpListenPort;           //监听端口


    //数据帧类型枚举体,作为公共接口暴露
     enum  MessageTypeEnum{
        RuningFData   = 0x10F9,
        RuningAccData = 0x10FA,
        RuningVelData = 0x10FB,
        RuningPosData = 0x10FC,
        F_DATA   = 0X10FD,
        ACC_DATA = 0x10FE,
        Vel_DATA = 0x10FF,
        POS_DATA = 0x1100 ,
        COMMAND = 0x1101 ,
        ControlMethod = 0x1102 ,
        PosPID = 0x1103,
        StaticPID = 0x1104,
        AccPID = 0x1105,
        PosRefData = 0x1106,
        VelRefData = 0x1107,
        AccRefData = 0x1108,
        SystemInfo = 0x1109,
        UploadData = 0x110A,
        ConstrainParam = 0x110B,
        EmegencyStop = 0x110C,
        ECHO = 0x110D,
        ERR = 0x110E ,
        ControlVarible = 0x110F,
        SineWaveParam = 0x1110,
        StaticVoltage = 0x1111,
        TEST = 0x1112,

    };

    enum  ServerPortEnum{
        COMMUNICATION_PORT = 8092,
        FILE_PORT = 40,
    };


   //构造不同结构的信息帧，返回值为构造的信息帧

    static QByteArray SendMsg(const ProtocolSet::MessageTypeEnum msg_type,void * msg,const qint32 msg_len);
    QByteArray send_Msg(ProtocolSet::MessageTypeEnum msg_type,QString msg);
    QByteArray payload;
    qint16     head;

   //数据帧头部长度 （字节数） 帧头（qint16） 2bytes + 长度（qint32） 4bytes + 功能码（qint16） 2bytes = 8bytes
    qint32 HEAD_LENGTH;

private:
   //内部的数据帧构造
    //QByteArray PosDataMsg(QString msg);
    QByteArray data_msg(QString msg);
    QByteArray command_msg(QString msg);
    QByteArray echo_msg(QString msg);
    QByteArray error_msg(QString msg);
    QByteArray test_msg(const QString &msg);


    static QByteArray DataMsg(void *msg,const qint32 msg_len);
    static QByteArray CommandMsg(void *msg,const qint32 msg_len);
    static QByteArray ControlMethodMsg(void *msg,const qint32 msg_len);
    static QByteArray PosPIDMsg(void *msg,const qint32 msg_len);
    static QByteArray StaticPIDMsg(void *msg,const qint32 msg_len);
    static QByteArray AccPIDMsg(void *msg,const qint32 msg_len);
    static QByteArray PosRefDataMsg(void *msg,const qint32 msg_len);
    static QByteArray VelRefDataMsg(void *msg,const qint32 msg_len);
    static QByteArray AccRefDataMsg(void *msg,const qint32 msg_len);
    static QByteArray SystemInfoMsg(void *msg,const qint32 msg_len);


    static QByteArray EchoMsg(void *msg,const qint32 msg_len);
    static QByteArray ErrorMsg(void *msg,const qint32 msg_len);
    static QByteArray ControlVaribleMsg(void *msg,const qint32 msg_len);
    static QByteArray SineWaveParamMsg(void *msg,const qint32 msg_len);
    static QByteArray StaticVoltageMsg(void *msg,const qint32 msg_len);

    static QByteArray TestMsg(void *msg,const qint32 msg_len);
};

#endif // PROTOCOL_H
