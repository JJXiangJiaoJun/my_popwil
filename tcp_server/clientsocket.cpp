#include "clientsocket.h"

#include <QApplication>
#include <QDataStream>
#include <QDebug>
#include <QByteArray>


//#define COMM_DEBUG


TcpSocket::TcpSocket(QObject *parent) :  QTcpSocket(parent)
{
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(deleteLater()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));


}

int ClientSocket::sm_totalConnectId = 0;

/**
 * @brief ClientSocket::ClientSocket
 * incoming函数中调用，创建一个新的套接字
 * @param parent
 * @param socketDescriptor  新的套接字
 */
ClientSocket::ClientSocket(QObject *parent, qintptr socketDescriptor)
{
    //设置当前套接字id
    parent = parent;

    m_connectId = ++sm_totalConnectId;

    m_tcpSocket  = new TcpSocket(this);
    //设置套接字

    //连接信号槽
    ///套接字读取数据绑定到 读取数据槽函数
    connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(SltReadyRead()));
    ///套接字连接信号绑定到
    connect(m_tcpSocket,SIGNAL(connected()),this,SLOT(SltConnected()));
    connect(m_tcpSocket,SIGNAL(disconnected()),this,SLOT(SltDisconnected()));


    if(!m_tcpSocket->setSocketDescriptor(socketDescriptor))
    {
         qDebug()<<"绑定套接字失败!!!";
         return;
    }

    //下面获取其ip地址与端口号
    this->ip = m_tcpSocket->peerAddress().toString();
    this->port = m_tcpSocket->peerPort();

    emit signalConnected(this->ip,this->port);
}

ClientSocket::~ClientSocket()
{

}


/**
 * @brief ClientSocket::Close
 */
void ClientSocket::Close()
{
    m_tcpSocket->close();
}
/**
 * @brief ClientSocket::Disconnect
 */
void ClientSocket::Disconnect()
{
    m_tcpSocket->disconnect();
}

void ClientSocket::SltConnected()
{
    qDebug()<<"ClientSocket::SltConnected";
    emit signalConnected(this->ip,this->port);
}

////////////
/// \brief ClientSocket::SltDisconnected
///
void ClientSocket::SltDisconnected()
{
    qDebug()<<"ClientSocket::SltDisconnected";
    emit signalDisConnected(this->ip,this->port,this->m_connectId);
}

/**
 * @brief ClientSocket::SltReadyRead
 * 读取解析函数
 */
void ClientSocket::SltReadyRead()
{
    //从缓冲区获取数据
    QByteArray recv_msg = m_tcpSocket->readAll();

#ifdef COMM_DEBUG
    //下面是解析消息的函数
    //定义parse函数
    if(reply.length()<0)
    {
        qDebug()<<"读取消息错误";
    }
    QString data(reply);
    qDebug()<<"读取到一次消息";
    qDebug()<<data;
#else
    /**
      首先将上次缓存加上这次数据,将包组合起来
     **/
    tcp_buffer.append(recv_msg);

    /**
      解析过程
      **/
    FrameLengthType  msg_len;
    FrameFuncType   msg_type;

    qint32 tot_len = tcp_buffer.size();

    while(tot_len>0)
    {
        //首先与QDatastream进行绑定
        QDataStream package(&tcp_buffer,QIODevice::ReadOnly);
        package.setByteOrder(QDataStream::BigEndian);
        package.setVersion(QDataStream::Qt_5_9);

        //不够包头的数据就不处理
        if(tot_len<qint32(PACKAGE_MINIMA_SIZE))
        {
            break;
        }

        //读取数据长度
        package >> msg_len;
        qDebug()<<"msg_len"<<msg_len;

        //如果数据长度不够那么等够了再来解析
        if((tot_len-ProtocolSet::FrameLegthLen)<msg_len)
        {
            break;
        }

        package >> msg_type;

        //下面根据功能字进行匹配
        switch (msg_type) {

        //命令帧
        case ProtocolSet::COMMAND:
            qDebug()<<"ParseCommandFrame\n";
            break;
        case ProtocolSet::POS_DATA:
            //绘图读取,传输过来的数据中均为double类型
            qDebug()<<"ParsePosDataFrame\n";
            ProcessPackage::ParsePosDataMsg(package,msg_len-ProtocolSet::FrameFuncLen);
            break;
        case ProtocolSet::ACC_DATA:
            qDebug()<<"ParseAccDataFrame\n";
            ProcessPackage::ParseAccDataMsg(package,msg_len-ProtocolSet::FrameFuncLen);
            break;
        case ProtocolSet::ERR:
            qDebug()<<"ParseErrFrame\n";
            break;
        case ProtocolSet::ECHO:
            qDebug()<<"ParseEchoFrame\n";
            break;
        default:
            qDebug()<<"Message not define\n";
            break;
        }

        //缓存接下来的数据包
        recv_msg = tcp_buffer.right(tot_len - (ProtocolSet::FrameLegthLen + msg_len));
        //更新数据包长度
        tot_len = recv_msg.size();
        //更新为解析下一个包
        tcp_buffer = recv_msg;


    }



#endif

}

/**
 * @brief ClientSocket::SendMessage
 * @param type
 * @param msg
 * @param msg_len
 */
void ClientSocket::SendMsg(ProtocolSet::MessageTypeEnum &type, void *msg, const qint32 msg_len)
{
    if(!m_tcpSocket->isOpen()) {return;}

    ProtocolSet protocolset;
    m_tcpSocket->write(protocolset.SendMsg(type,msg,msg_len));
}


/**
 * @brief ClientSocket::SltSendMessage
 * 套接字中发送消息
 * @param type
 * @param data
 */
void ClientSocket::SltSendMessage(ProtocolSet::MessageTypeEnum &type, QString &data)
{
    if(!m_tcpSocket->isOpen())  {return;}


    //构建数据报
    ProtocolSet msg;
//    QByteArray sendbuf;

//    sendbuf = msg.send_Msg(type,data);
//    qDebug()<<sendbuf;

    m_tcpSocket->write(msg.send_Msg(type,data));
}
/**
 * @brief ClientSocket::setIP
 * @param ip
 */
void ClientSocket::setIP(QString ip)
{
    this->ip= ip;
}
/**
 * @brief ClientSocket::setPort
 * @param port
 */
void ClientSocket::setPort(int port)
{
    this->port = port;
}
/**
 * @brief ClientSocket::getIP
 * @return 当前连接的ip
 */
QString ClientSocket::getIP()
{
    return this->ip;
}
/**
 * @brief ClientSocket::getPort
 * @return 当前连接的端口号
 */
int ClientSocket::getPort()
{
    return this->port;
}
/**
 * @brief ClientSocket::getId
 * @return  获得当前连接的Id
 */
int ClientSocket::getId()
{
    return this->m_connectId;
}

