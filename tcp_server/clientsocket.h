#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QtGlobal>
#include <QString>
#include <QByteArray>
#include <QThread>

#include "protocol.h"
#include "process_package.h"
#include "global_setting.h"
#include "globaldata.h"


//extern QByteArray tcp_buffer;

//****************************************TCP多线程编程，需要重写 QTcpsocket类 和 QTcpServerAbstract类***********************************
//****************************************incomingconnect函数需要重写，在其中获得套接字****************************************

class TcpSocket : public QTcpSocket
{
    Q_OBJECT
public:
    explicit TcpSocket(QObject *parent = 0);
private slots:

signals:
//    void SigsendData(const QString &ip, int port, const QString &data);
//    void SigreceiveData(const QString &ip, int port, const QString &data);

public slots:
};

/////////////////////////////////////////////////////////////////////
/// \brief The ClientSocket class
///服务器socket管理类
class ClientSocket : public QObject
{
    Q_OBJECT
public:
    explicit ClientSocket(QObject *parent=0, qintptr socketDescriptor=0);
    ~ClientSocket();

//***********************静态变量**************************
    static int sm_totalConnectId;  //总连接数，每次来一个连接就+1

//***********************公有成员变量**************************
    TcpSocket *m_tcpSocket;

//***********************公有成员函数**************************
    void Close();
    void Disconnect();

    void setIP(QString ip);
    void setPort(int port);
    QString getIP();
    int getPort();
    int getId();

    void SendMsg(ProtocolSet::MessageTypeEnum &type,void *msg,const qint32 msg_len);


//***********************私有成员变量**************************
private:
    int m_connectId;             //保存到当前连接套接字的id
    QString ip;
    int port;



// 信号和槽函数*************************************************
signals:
    void signalConnected(QString ip,int port);
    void signalDisConnected(QString ip,int port,int disconnId);
    void signalMsgToClient(ProtocolSet::MessageTypeEnum &type,const int &id);

public slots:
    //消息回发
    void SltSendMessage(ProtocolSet::MessageTypeEnum &type,QString &data);
private slots:
    void SltConnected();
    void SltDisconnected();
    void SltReadyRead();

// 信号和槽函数*************************************************

};

#endif // CLIENTSOCKET_H
