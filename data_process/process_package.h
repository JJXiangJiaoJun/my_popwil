#ifndef PROCESS_PACKAGE_H
#define PROCESS_PACKAGE_H

#include <QByteArray>
#include <QtGlobal>
#include <QDataStream>
#include <QIODevice>
#include <QDebug>

#include "globaldata.h"
#include "global_setting.h"



/**
 * @brief The ProcessPackage class
 * 消息的处理类
 */
class ProcessPackage
{
public:
    //用于Debug调试，将传输过来的数据都进行显示
    static void ParseDebugPosDataMsg(QDataStream &raw_data,FrameLengthType msg_len);
    static void ParseDebugVelDataMsg(QDataStream &raw_data,FrameLengthType msg_len);
    static void ParseDebugAccDataMsg(QDataStream &raw_data,FrameLengthType msg_len);
    static void ParseDebugFDataMsg(QDataStream &raw_data,FrameLengthType msg_len);
    //实际的还未开始试验的操作，每次都显示10ms间隔的点
    static void ParseCurPosDataMsg(QDataStream &raw_data,FrameLengthType msg_len);
    static void ParseCurVelDataMsg(QDataStream &raw_data,FrameLengthType msg_len);
    static void ParseCurAccDataMsg(QDataStream &raw_data,FrameLengthType msg_len);
    static void ParseCurFDataMsg(QDataStream &raw_data,FrameLengthType msg_len);
    //开始试验的之后操作，每次都显示10ms间隔的点
    static void ParseRunningPosDataMsg(QDataStream &raw_data,FrameLengthType msg_len);
    static void ParseRunningVelDataMsg(QDataStream &raw_data,FrameLengthType msg_len);
    static void ParseRunningAccDataMsg(QDataStream &raw_data,FrameLengthType msg_len);
    static void ParseRunningFDataMsg(QDataStream &raw_data,FrameLengthType msg_len);

    //回复帧
    static void ParseCommandMsg(QDataStream &raw_data,FrameLengthType msg_len);
    static void ParseEchoMsg(QDataStream &raw_data,FrameLengthType msg_len);
    //错误信息帧
    static void ParseErrorMsg(QDataStream &raw_data,FrameLengthType msg_len);
};

#endif // PROCESS_PACKAGE_H
