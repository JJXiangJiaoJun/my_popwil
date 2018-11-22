#include <QDebug>
#include "protocol.h"



ProtocolSet::ProtocolSet():head(0xAAAA),HEAD_LENGTH(8)

{

}

////////////////////////////////////////////////////////////////////
/// \brief ProtocolSet::send_Msg
/// \param msg_type 消息类型
/// \param msg
/// \return
///发送消息数据报

QByteArray ProtocolSet::send_Msg(MessageTypeEnum msg_type, QString msg)
{
    //***********************根据不同的msg_type发送不同的数据包******************
    qDebug()<<"开始构造数据包: "<<msg_type;
    QByteArray message;
    switch (msg_type) {
    case DATA:
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




QByteArray ProtocolSet::data_msg(QString msg)
{
    //数据总长度
//    qint32 tot_len = 0;
//    //暂存需要发送的数据
//    QByteArray pbuf;

//    //使用数据流写入数据,绑定pbuf
//    QDataStream output(&pbuf,QIODevice::WriteOnly);
//    //获取发送消息的总长度
//    tot_len = msg.toUtf8().size()+HEAD_LENGTH;
//    //*************接下来构造数据包  格式为  0xAAAA + tot_len + fun + data************
//    output<<qint16(head)<<qint32(tot_len)<<qint16(ProtocolSet::DATA)<<msg.toUtf8();


    QByteArray pbuf;
    QDataStream out(&pbuf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    //构造消息类型数据报
    out <<quint16(0)<<qint16(ProtocolSet::DATA)<<msg.toLatin1();
    out.device()->seek(0);
    out<<quint16(pbuf.size())-sizeof(quint16);

    //返回构造的数据包
    return pbuf;

}

QByteArray ProtocolSet::command_msg(QString msg)
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
//    output<<qint16(head)<<qint32(tot_len)<<qint16(ProtocolSet::COMMAND)<<msg.toUtf8();

    QByteArray pbuf;
    QDataStream out(&pbuf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    //构造消息类型数据报
    out <<quint16(0)<<qint16(ProtocolSet::COMMAND)<<msg.toLatin1();
    out.device()->seek(0);
    out<<quint16(pbuf.size())-sizeof(quint16);

    //返回构造的数据包
    return pbuf;
}

QByteArray ProtocolSet::echo_msg(QString msg)
{
    //数据总长度
//    qint32 tot_len = 0;
//    //暂存需要发送的数据
//    QByteArray pbuf;

//    //使用数据流写入数据,绑定pbuf
//    QDataStream output(&pbuf,QIODevice::WriteOnly);
//    //获取发送消息的总长度
//    tot_len = msg.toUtf8().size()+HEAD_LENGTH;
//    //*************接下来构造数据包  格式为  0xAAAA + tot_len + fun + data************
//    output<<qint16(head)<<qint32(tot_len)<<qint16(ProtocolSet::ECHO)<<msg.toUtf8();

    QByteArray pbuf;
    QDataStream out(&pbuf,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_9);

    //构造消息类型数据报
    out <<quint16(0)<<qint16(ProtocolSet::ECHO)<<msg.toLatin1();
    out.device()->seek(0);
    out<<quint16(pbuf.size())-sizeof(quint16);

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
    out<<quint16(pbuf.size())-sizeof(quint16);

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
