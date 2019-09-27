#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QtGlobal>
#include <QObject>
#include <QWidget>


#include "global_setting.h"
//////////////////////////////////////////////////////////////////////////////
/// \brief The ProtocolSet class
///构造数据报的协议
///

#define PROTOCOL_HEAD 0xAAAA
#define PACKAGE_MINIMA_SIZE sizeof(quint16)+sizeof(qint16)



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
        F_DATA = 0x10FE,
        ACC_DATA = 0x10FF,
        POS_DATA = 0x1100 ,
        COMMAND = 0x1101 ,
        ECHO = 0x1102 ,
        ERR = 0x1103 ,
        WARNING = 0x1104,
        TEST = 0x1105,
        PARAM = 0x1106,
    };

    enum  ServerPortEnum{
        COMMUNICATION_PORT = 8092,
        FILE_PORT = 40,
    };



   //构造不同结构的信息帧，返回值为构造的信息帧

    QByteArray SendMsg(const ProtocolSet::MessageTypeEnum msg_type,void * msg,const qint32 msg_len);
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


    QByteArray DataMsg(void *msg,const qint32 msg_len);
    QByteArray CommandMsg(void *msg,const qint32 msg_len);
    QByteArray EchoMsg(void *msg,const qint32 msg_len);
    QByteArray ErrorMsg(void *msg,const qint32 msg_len);
    QByteArray TestMsg(void *msg,const qint32 msg_len);

    QByteArray ExperimentParamMsg(void *msg,const qint32 msg_len);

};

#endif // PROTOCOL_H
