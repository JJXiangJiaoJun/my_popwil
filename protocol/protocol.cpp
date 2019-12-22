#include "protocol.h"

#include <QDebug>
#include <QtEndian>

FrameLengthType ProtocolSet::FrameLegthLen = sizeof(FrameLengthType);
FrameFuncType ProtocolSet::FrameFuncLen = sizeof(FrameFuncType);



ProtocolSet::ProtocolSet():head(0xAAAA),HEAD_LENGTH(8)

{

}

////////////////////////////////////////////////////////////////////
/// \brief ProtocolSet::send_Msg
/// \param msg_type 消息类型
/// \param msg
/// \return
///发送消息数据报
///
/**
 * @brief SendMsg  根据 msg_type 构造数据包
 * @param msg_type 发送消息数据类型
 * @param msg      真实数据指针
 * @return
 */
QByteArray ProtocolSet::SendMsg(const MessageTypeEnum msg_type,void * msg,const qint32 msg_len)
{

    qDebug()<<"开始构造数据包: "<<msg_type;
    QByteArray message;

    switch (msg_type) {
    case POS_DATA:
        message = DataMsg(msg,msg_len);
        break;
    case COMMAND:
        message = CommandMsg(msg,msg_len);
        break;
    case ECHO:
        message = EchoMsg(msg,msg_len);
        break;
    case ERR:
        message = ErrorMsg(msg,msg_len);
        break;
    case TEST:
        message = TestMsg(msg,msg_len);
        break;
    case PARAM:
        message = ExperimentParamMsg(msg,msg_len);
        break;
    default:
        qDebug()<<"错误！发送数据类型不正确";
        break;
    }

    if(message.size()>0)
        qDebug()<<"构造数据包成功:"<<message.size()<<"Bytes";
    else
        qDebug()<<"不能成功构造数据";


    return message;
}

QByteArray ProtocolSet::send_Msg(MessageTypeEnum msg_type, QString msg)
{
    //***********************根据不同的msg_type发送不同的数据包******************
    qDebug()<<"开始构造数据包: "<<msg_type;
    QByteArray message;
    switch (msg_type) {
    case POS_DATA:
        message = data_msg(msg);
        break;
    case COMMAND:
        message = command_msg(msg);
        break;
    case ECHO:
        message = echo_msg(msg);
        break;
    case ERR:
        message = error_msg(msg);
        break;
    case TEST:
        message = test_msg(msg);
        break;
    default:
        qDebug()<<"错误！发送数据类型不正确";
        break;
    }
    if(message.size()>0)
        qDebug()<<"发送成功:"<<message.size()<<"Bytes";
    else
        qDebug()<<"未成功发送数据";


    //返回所构造的数据包
    return message;
}


QByteArray ProtocolSet::DataMsg(void *msg, const qint32 msg_len)
{

}

QByteArray ProtocolSet::CommandMsg(void *msg, const qint32 msg_len)
{

}


QByteArray ProtocolSet::EchoMsg(void *msg, const qint32 msg_len)
{

}

QByteArray ProtocolSet::ErrorMsg(void *msg, const qint32 msg_len)
{

}


QByteArray ProtocolSet::ExperimentParamMsg(void *msg, const qint32 msg_len)
{
    //解析数据指针
    QByteArray pbuf;
    if(msg_len <=0) return pbuf;
    ExperimentParamStruct *paramMsg_prt = (ExperimentParamStruct *)msg;


    qDebug() << "构造数据包幅值" << paramMsg_prt->amplitude << "构造数据包相位" << paramMsg_prt->frequency;

    QDataStream out(&pbuf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);
    out.setByteOrder(QDataStream::BigEndian);
    //构造消息数据包
    quint16 tot_len = 0;
    out << tot_len << qint16(PARAM) << qint16(paramMsg_prt->waveform) << \
           double(paramMsg_prt->amplitude) << double(paramMsg_prt->frequency);
    out.device()->seek(0);
    out << quint16(pbuf.size()-FrameLegthLen);

    return pbuf;
}

QByteArray ProtocolSet::TestMsg(void *msg, const qint32 msg_len)
{

}


QByteArray ProtocolSet::data_msg(QString msg)
{
    //数据总长度
//    //*************接下来构造数据包  格式为  0xAAAA + tot_len + fun + data************
//    output<<qint16(head)<<qint32(tot_len)<<qint16(ProtocolSet::POS_DATA)<<msg.toUtf8();


    QByteArray pbuf;
    QDataStream out(&pbuf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);


    //构造消息类型数据报
    out <<quint16(0)<<qint16(ProtocolSet::POS_DATA)<<msg.toLatin1();
    out.device()->seek(0);
    out<<quint16(pbuf.size()-sizeof(quint16));

    //返回构造的数据包
    return pbuf;

}

QByteArray ProtocolSet::command_msg(QString msg)
{

//    //*************接下来构造数据包  格式为  0xAAAA + tot_len + fun + data************
//    output<<qint16(head)<<qint32(tot_len)<<qint16(ProtocolSet::COMMAND)<<msg.toUtf8();

    QByteArray pbuf;
    QDataStream out(&pbuf,QIODevice::WriteOnly);
    //网络通信默认使用大端模式
    out.setByteOrder(QDataStream::BigEndian);
    out.setVersion(QDataStream::Qt_5_9);

    quint8 data;


    if(msg=="start")
        data = 1;
    else if(msg=="stop")
        data = 2;
   else{
        data=3;
        printf("Warning not Implement");
      }

    quint16 tot_len = 0;
   //qint16 head = PROTOCOL_HEAD;

    //构造消息类型数据报
    //*************接下来构造数据包  格式为  0xAAAA + tot_len + fun + data************
//    out <<head<<tot_len<<qint16(ProtocolSet::COMMAND)<<data;
    out <<tot_len<<qint16(ProtocolSet::COMMAND)<<data;
//    out.device()->seek(2);
    out.device()->seek(0);
    out<<quint16(pbuf.size()-sizeof(quint16));

    qDebug()<<pbuf;
    //返回构造的数据包
    return pbuf;
}

QByteArray ProtocolSet::echo_msg(QString msg)
{

//    //*************接下来构造数据包  格式为  0xAAAA + tot_len + fun + data************
//    output<<qint16(head)<<qint32(tot_len)<<qint16(ProtocolSet::ECHO)<<msg.toUtf8();

    QByteArray pbuf;
    QDataStream out(&pbuf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    quint16 tot_len = 0;
   // qint16 head = PROTOCOL_HEAD;

    //构造消息类型数据报
    out <<tot_len<<qint16(ProtocolSet::ECHO)<<msg.toLatin1();
    out.device()->seek(0);
    out<<quint16(pbuf.size()-sizeof(quint16));

    //返回构造的数据包
    return pbuf;
}

QByteArray ProtocolSet::error_msg(QString msg)
{
//    //数据总长度
//    qint32 tot_len = 0;
//    //暂存需要发送的数据
//    QByteArray pbuf;

//    //使用数据流写入数据,绑定pbuf
//    QDataStream output(&pbuf,QIODevice::WriteOnly);
//    //获取发送消息的总长度
//    tot_len = msg.toUtf8().size()+HEAD_LENGTH;
//    //*************接下来构造数据包  格式为  0xAAAA + tot_len + fun + data************
//    output<<qint16(head)<<qint32(tot_len)<<qint16(ProtocolSet::ERR)<<msg.toUtf8();
    QByteArray pbuf;
    QDataStream out(&pbuf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    //构造消息类型数据报
    out <<quint16(0)<<qint16(ProtocolSet::ERR)<<msg.toLatin1();
    out.device()->seek(0);
    out<<quint16(pbuf.size()-sizeof(quint16));

    //返回构造的数据包
    return pbuf;
}


QByteArray ProtocolSet::test_msg(const QString &msg)
{

    QByteArray pbuf;
    QDataStream out(&pbuf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    //直接发送消息字符串
    out <<msg.toLatin1();

    //返回构造的数据包
    return pbuf;
}
