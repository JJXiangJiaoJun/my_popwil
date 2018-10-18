#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <QtGlobal>
#include <QObject>
#include <QWidget>
//////////////////////////////////////////////////////////////////////////////
/// \brief The ProtocolSet class
///构造数据报的协议
class ProtocolSet
{
public:
    ProtocolSet();
//    static QString ConfigFile;          //配置文件路径
//    static QString SendFileName;        //发送配置文件名
//    static QString DeviceFileName;      //模拟设备数据文件名

//    //TCP服务器配置参数
//    static bool HexSendTcpServer;       //16进制发送
//    static bool HexReceiveTcpServer;    //16进制接收
//    static bool AsciiTcpServer;         //ASCII模式
//    static bool DebugTcpServer;         //启用数据调试
//    static bool AutoSendTcpServer;      //自动发送数据
//    static int IntervalTcpServer;       //发送数据间隔
//    static int TcpListenPort;           //监听端口


//    //读写配置参数及其他操作
//    static void readConfig();           //读取配置参数
//    static void writeConfig();          //写入配置参数
//    static void newConfig();            //以初始值新建配置文件
//    static bool checkConfig();          //校验配置文件

//    static QStringList Intervals;
//    static QStringList Datas;
//    static QStringList Keys;
//    static QStringList Values;
//    static void readSendData();
//    static void readDeviceData();
public:
    //数据帧类型枚举体,作为公共接口暴露
    enum  MessageType{
        DATA = 0xFF00 ,
        COMMAND = 0xFF01 ,
        ECHO = 0xFF02 ,
        ERR = 0xFF03 ,
        WARNING = 0xFF04,
        TEST = 0xFF05,
        PARA = 0xFF06,
    };

    enum  ServerPort{
        COMMUNICATION_PORT = 8087,
        FILE_PORT = 40,
    };

public:
   //构造不同结构的信息帧，返回值为构造的信息帧
    QByteArray send_Msg(ProtocolSet::MessageType msg_type,QString msg);
    QByteArray payload;
    qint16 head;

   //数据帧头部长度 （字节数） 帧头（qint16） 2bytes + 长度（qint32） 4bytes + 功能码（qint16） 2bytes = 8bytes
    qint32 HEAD_LENGTH;

private:
   //内部的数据帧构造
    QByteArray data_msg(QString msg);
    QByteArray command_msg(QString msg);
    QByteArray echo_msg(QString msg);
    QByteArray error_msg(QString msg);
    QByteArray test_msg(const QString &msg);

};

#endif // PROTOCOL_H
