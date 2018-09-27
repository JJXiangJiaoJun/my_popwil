#include "protocol.h"
#include <QDebug>


ProtocolSet::ProtocolSet():head(0xAAAA),HEAD_LENGTH(8)

{

}

QByteArray ProtocolSet::send_Msg(MessageType msg_type, QString msg)
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
    qint32 tot_len = 0;
    //暂存需要发送的数据
    QByteArray pbuf;

    //使用数据流写入数据,绑定pbuf
    QDataStream output(&pbuf,QIODevice::WriteOnly);
    //获取发送消息的总长度
    tot_len = msg.toUtf8().size()+HEAD_LENGTH;
    //*************接下来构造数据包  格式为  0xAAAA + tot_len + fun + data************
    output<<qint16(head)<<qint32(tot_len)<<qint16(ProtocolSet::DATA)<<msg.toUtf8();

    //返回构造的数据包
    return pbuf;

}

QByteArray ProtocolSet::command_msg(QString msg)
{
    //数据总长度
    qint32 tot_len = 0;
    //暂存需要发送的数据
    QByteArray pbuf;

    //使用数据流写入数据,绑定pbuf
    QDataStream output(&pbuf,QIODevice::WriteOnly);
    //获取发送消息的总长度
    tot_len = msg.toUtf8().size()+HEAD_LENGTH;
    //*************接下来构造数据包  格式为  0xAAAA + tot_len + fun + data************
    output<<qint16(head)<<qint32(tot_len)<<qint16(ProtocolSet::COMMAND)<<msg.toUtf8();

    //返回构造的数据包
    return pbuf;
}

QByteArray ProtocolSet::echo_msg(QString msg)
{
    //数据总长度
    qint32 tot_len = 0;
    //暂存需要发送的数据
    QByteArray pbuf;

    //使用数据流写入数据,绑定pbuf
    QDataStream output(&pbuf,QIODevice::WriteOnly);
    //获取发送消息的总长度
    tot_len = msg.toUtf8().size()+HEAD_LENGTH;
    //*************接下来构造数据包  格式为  0xAAAA + tot_len + fun + data************
    output<<qint16(head)<<qint32(tot_len)<<qint16(ProtocolSet::ECHO)<<msg.toUtf8();

    //返回构造的数据包
    return pbuf;
}

QByteArray ProtocolSet::error_msg(QString msg)
{
    //数据总长度
    qint32 tot_len = 0;
    //暂存需要发送的数据
    QByteArray pbuf;

    //使用数据流写入数据,绑定pbuf
    QDataStream output(&pbuf,QIODevice::WriteOnly);
    //获取发送消息的总长度
    tot_len = msg.toUtf8().size()+HEAD_LENGTH;
    //*************接下来构造数据包  格式为  0xAAAA + tot_len + fun + data************
    output<<qint16(head)<<qint32(tot_len)<<qint16(ProtocolSet::ERR)<<msg.toUtf8();

    //返回构造的数据包
    return pbuf;
}


QByteArray ProtocolSet::test_msg(const QString &msg)
{
    //数据总长度
    qint32 tot_len = 0;
    //暂存需要发送的数据
    QByteArray pbuf;
    //使用数据流写入数据,绑定pbuf
    QDataStream output(&pbuf,QIODevice::WriteOnly);
    //获取发送消息的总长度

    tot_len = msg.toUtf8().size()+HEAD_LENGTH;
    //*************接下来构造数据包  格式为  0xAAAA + tot_len + fun + data************
    output<<qint16(head)<<qint32(tot_len)<<qint16(ProtocolSet::TEST);
    pbuf.append(msg);
    //output.device()->seek(pbuf.size());
    //output<<0x01f2;
    qDebug()<<"to Latin1"<<msg.toLatin1();
    qDebug()<<"数据包为"<<pbuf;
    //返回构造的数据包
    return pbuf;
}
