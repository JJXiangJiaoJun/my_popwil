#ifndef TCPSERVER_H
#define TCPSERVER_H

#include <QtNetwork>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>

#include "protocol.h"
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

private:
    QList<TcpClient *> clients;

protected:
    void incomingConnection(qintptr handle);

private slots:
    void disconnected();

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

#endif // TCPSERVER_H
