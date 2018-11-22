#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

#include "protocol.h"
#include "clientsocket.h"
#include "serverthread.h"

//****************************************TCP多线程编程，需要重写 QTcpsocket类 和 QTcpserver类***********************************
//****************************************incomingconnect函数需要重写，在其中获得套接字****************************************
/**
 * @brief The TcpServerAbstract class
 * 重写的QTcpServer类，由此类扩展为各个服务器
 *
 *
 * 实现功能：1、连接客户端 2、断开客户端连接
 * 具体发送、接收数据的功能交给子类实现
 */

class TcpServerAbstract : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServerAbstract(QObject *parent = 0);
    ~TcpServerAbstract();

    //开始与结束服务器的侦听
    bool    StartListen(int port = 6666);
    void    CloseListen();

    //获取当前服务器套接字连接列表的引用
    QList<ClientSocket *> &get_ConnectClients();

protected:
    //重写此函数，在此函数中开启多线程
    void incomingConnection(qintptr handle);

private:
    //保存连接的套接字
    QList<ClientSocket *> m_clients;

signals:
     //**********************发送和接收数据的信号函数***********
    void sendData(const QString &ip, int port, const QString &data);
    void receiveData(const QString &ip, int port, const QString &data);
    //******************************************

    void clientConnected(const QString &ip, int port);
    void clientDisconnected(const QString &ip, int port);

    void signalUserStatus(const QString &text);

public slots:



protected slots:
    //继承的虚函数
    virtual void SltNewConnection()=0;
    virtual void SltConnected()=0;
    virtual void SltDisConnected(const QString &ip,const int &port,const int disconnId)=0;

    //待实现，子类通过实现该函数从而实现自己的发送数据功能。

    //virtual void sendMsg();
};

//////////////////////////////////////////////////////////////////////////////
/**
 * @brief The TcpMsgServer class
 * 消息服务器，继承TcpServerAbstract类
 */
class TcpMsgServer :public TcpServerAbstract{
    Q_OBJECT
public:
    explicit TcpMsgServer(QObject *parent=0);
    ~TcpMsgServer();


private:

signals:

public slots:
    void SltMsgToClient(ProtocolSet::MessageTypeEnum type,QString &msg);
//重写的虚函数
private slots:
    void SltNewConnection();
    void SltConnected();
    void SltDisConnected(const QString &ip,const int &port,const int disconnId);
};




#endif // TCPSERVER_H
