#include "clientsocket.h"

#include <QApplication>
#include <QDataStream>
#include <QDebug>
#include <QByteArray>

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
//    if(m_tcpSocket->waitForConnected())
//    {
//        qDebug()<<"连接成功!";
//    }
//    else
//    {
//        qDebug()<<"连接失败!";
//        return;
//    }
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
    QByteArray reply = m_tcpSocket->readAll();

    //下面是解析消息的函数
    if(reply.length()<0)
    {
        qDebug()<<"读取消息错误";
    }
    QString data(reply);
    qDebug()<<"读取到一次消息";
    qDebug()<<data;


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
    QByteArray sendbuf;

    sendbuf = msg.send_Msg(type,data);
    qDebug()<<sendbuf;

    m_tcpSocket->write(sendbuf);
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

