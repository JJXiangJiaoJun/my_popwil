#ifndef CLIENTSOCKET_H
#define CLIENTSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "protocol.h"
#include <QTcpServer>
#include <QtGlobal>
#include <QString>
//****************************************TCP多线程编程，需要重写 QTcpsocket类 和 QTcpserver类***********************************
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

    int GetUserId() const;
    void Close();
    void Disconnect();
signals:
    void signalConnected(QString ip,int port);
    void signalDisConnected(QString ip,int port);
    void signalMsgToClient(ProtocolSet::MessageType &type,const int &id);
public slots:

public:
    TcpSocket *m_tcpSocket;

private:
    int m_nId;
    QString ip;
    int port;
public:
    void setIP(QString ip);
    void setPort(int port);
    QString getIP();
    int getPort();
    int getId();
public slots:
    //消息回发
    void SltSendMessage(ProtocolSet::MessageType &type,QString &data);
private slots:
    void SltConnected();
    void SltDisconnected();
    void SltReadyRead();


private:
   //客户端上来的消息解析功能

};

#endif // CLIENTSOCKET_H
