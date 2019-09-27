#ifndef PROCESS_PACKAGE_H
#define PROCESS_PACKAGE_H

#include <QByteArray>
#include <QtGlobal>
#include <QDataStream>
#include <QIODevice>


#include "globaldata.h"
#include "global_setting.h"



/**
 * @brief The ProcessPackage class
 * 消息的处理类
 */
class ProcessPackage
{
public:
    static void ParsePosDataMsg(QDataStream &raw_data,quint16 msg_len);
    static void ParseAccDataMsg(QDataStream &raw_data,quint16 msg_len);
    static void ParseCommandMsg(QDataStream &raw_data,quint16 msg_len);
};

#endif // PROCESS_PACKAGE_H
