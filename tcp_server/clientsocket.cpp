#include "clientsocket.h"

#include <QApplication>
#include <QDataStream>
#include <QDebug>
#include <QByteArray>
TcpSocket::TcpSocket(QObject *parent) :  QTcpSocket(parent)
{
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(deleteLater()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
    connect(this, SIGNAL(disconnected()), this, SLOT(disconnected()));

}

/**
 * @brief ClientSocket::ClientSocket
 * incoming函数中调用，创建一个新的套接字
 * @param parent
 * @param socketDescriptor  新的套接字
 */
ClientSocket::ClientSocket(QObject *parent, qintptr socketDescriptor)
{
    m_nId = -1;
    //设置套接字
    m_tcpSocket->setSocketDescriptor(socketDescriptor);
    //连接信号槽
    connect(m_tcpSocket,SIGNAL(readyRead()),this,SLOT(SltReadyRead()));
    connect(m_tcpSocket,SIGNAL(connected()),this,SLOT(SltConnected()));
    connect(m_tcpSocket,SIGNAL(disconnected()),this,SLOT(SltDisconnected()));

    connect(m_tcpSocket,SIGNAL(connected()),this,SIGNAL(signalConnected()));
    connect(m_tcpSocket,SIGNAL(disconnected()),this,SIGNAL(signalDisConnected()));

}

ClientSocket::~ClientSocket()
{

}

int ClientSocket::GetUserId() const
{
    return m_nId;
}

void ClientSocket::Close()
{
    m_tcpSocket->abort();
}

void ClientSocket::SltConnected()
{
    qDebug()<<"connected";
    emit signalConnected();
}

////////////
/// \brief ClientSocket::SltDisconnected
///
void ClientSocket::SltDisconnected()
{
    qDebug()<<"disconnected";
    emit signalDisConnected();
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
void ClientSocket::SltSendMessage(ProtocolSet::MessageType &type, QString &data)
{
    if(!m_tcpSocket->isOpen())  {return;}

    //构建数据报
    ProtocolSet msg;
    QByteArray sendbuf;
    sendbuf = msg.send_Msg(type,data);
    m_tcpSocket->write(sendbuf);
}

void ClientSocket::setIP(QString ip)
{
    this->ip= ip;
}

void ClientSocket::setPort(int port)
{
    this->port = port;
}
