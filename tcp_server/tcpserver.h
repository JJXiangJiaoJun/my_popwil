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
 * @brief The TcpServer class
 * 重写的QTcpServer类，由此类扩展为各个服务器
 */
class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);
    ~TcpServer();
public:
    //开始与结束服务器的侦听
    bool  StartListen(int port = 6666);
    void  CloseListen();
private:
    //保存连接的套接字
    QList<ClientSocket *> m_clients;

protected:
    //重写此函数，在此函数中开启多线程
    void incomingConnection(qintptr handle);


public:
    QList<ClientSocket *> &get_ConnectClients();

private slots:
     void Sltdisconnected();
     void Sltdisconnected(QString &ip,int &port);

signals:
    void sendData(const QString &ip, int port, const QString &data);
    void receiveData(const QString &ip, int port, const QString &data);

    void clientConnected(const QString &ip, int port);
    void clientDisconnected(const QString &ip, int port);

    void signalUserStatus(const QString &text);

public slots:

protected slots:
    //继承的虚函数
    virtual void SltNewConnection()=0;
    virtual void SltConnected()=0;
    virtual void SltDisConnected()=0;
};


/////////////////////////////////////////////////////////////////////////
///// \brief The TCPServer class
/////服务器管理类

//class TCPServer :public QObject{
//    Q_OBJECT
//public:
//    explicit TCPServer(QObject *parent=0);
//    ~TCPServer();


//    bool  StartListen(int port = 6666);
//    void  CloseListen();
//signals:
//    void signalUserStatus(const QString &text);
//protected:
//    TcpServer *m_tcpServer;
//public slots:
//protected slots:

//    //继承的虚函数
//    virtual void SltNewConnection()=0;
//    virtual void SltConnected()=0;
//    virtual void SltDisConnected()=0;
//};

//////////////////////////////////////////////////////////////////////////////
/**
 * @brief The TcpMsgServer class
 * 消息服务器，继承TcpServer类
 */
class TcpMsgServer :public TcpServer{
    Q_OBJECT
public:
    explicit TcpMsgServer(QObject *parent=0);
    ~TcpMsgServer();

signals:
    void newconnect_client(const QString &ip, int port);
    void disconnect_client(const QString &ip, int port);
private:

public slots:

private slots:
    void SltNewConnection();
    void SltConnected();
    void SltDisConnected();
public slots:
    void SltMsgToClient(ProtocolSet::MessageType type,QString &msg);
};




#endif // TCPSERVER_H
