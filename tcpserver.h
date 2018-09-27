#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

#include "protocol.h"
#include "clientsocket.h"
//****************************************TCP多线程编程，需要重写 QTcpsocket类 和 QTcpserver类***********************************
//****************************************incomingconnect函数需要重写，在其中获得套接字****************************************
class TcpClient : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpClient(QObject *parent = 0);

private:
    QString ip;
    int port;

private slots:
    void connected();
    void disconnected();
    void readData();

signals:
    void sendData(const QString &ip, int port, const QString &data);
    void receiveData(const QString &ip, int port, const QString &data);

public slots:
    void setIP(const QString &ip);
    void setPort(int port);
    void sendData(const QString &data);

    void sendData(const QString &data,ProtocolSet::MessageType type);

};



class TcpServer : public QTcpServer
{
    Q_OBJECT
public:
    explicit TcpServer(QObject *parent = 0);

public:
    QList<TcpClient *> clients;

public:
    QVector<ClientSocket *> m_clients;
protected:
    void incomingConnection(qintptr handle);

private slots:
    void disconnected();
    void Sltdisconnected();
signals:
    void sendData(const QString &ip, int port, const QString &data);
    void receiveData(const QString &ip, int port, const QString &data);

    void clientConnected(const QString &ip, int port);
    void clientDisconnected(const QString &ip, int port);

public slots:
    //启动服务
    bool start();
    //停止服务
    void stop();

    //指定连接发送数据
    void writeData(const QString &ip, int port, const QString &data);
    //对所有连接发送数据
    void writeData(const QString &data);

    void writeData(const QString &data,ProtocolSet::MessageType type);


};


///////////////////////////////////////////////////////////////////////
/// \brief The TCPServer class
///服务器管理类

class TCPServer :public QObject{
    Q_OBJECT
public:
    explicit TCPServer(QObject *parent=0);
    ~TCPServer();


    bool  StartListen(int port = 6666);
    void  CloseListen();
signals:
    void signalUserStatus(const QString &text);
protected:
    TcpServer *m_tcpServer;
public slots:
protected slots:

    //继承的虚函数
    virtual void SltNewConnection()=0;
    virtual void SltConnected()=0;
    virtual void SltDisConnected()=0;
};

//////////////////////////////////////////////////////////////////////////////
/// \brief The TcpMsgServer class
///消息服务器
class TcpMsgServer :public TCPServer{
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
