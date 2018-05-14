#include "tcpserver.h"
#include <QtWidgets>
#include <QHostAddress>
#include <QDebug>
TcpClient::TcpClient(QObject *parent) :  QTcpSocket(parent)
{
    ip = "192.168.1.3";
    port = 8087;

    connect(this, SIGNAL(connected()), this, SLOT(connected()));
    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(deleteLater()));
    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
    connect(this, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(this, SIGNAL(readyRead()), this, SLOT(readData()));
}

void TcpClient::connected()
{
    emit sendData(ip, port, "客户端上线");
    qDebug()<<"客户端上线";
}

void TcpClient::disconnected()
{
    emit sendData(ip, port, "客户端下线");
    qDebug()<<"客户端下线";
}

void TcpClient::readData()
{
    //TCP字符流连接
    QByteArray data = this->readAll();
    if (data.length() <= 0) {
        return;
    }

    QString buffer;
//    if (App::HexReceiveTcpServer) {
//        buffer = QUIHelper::byteArrayToHexStr(data);
//    } else if (App::AsciiTcpServer) {
//        buffer = QUIHelper::byteArrayToAsciiStr(data);
//    } else {
//        buffer = QString(data);
//    }
    buffer = QString(data);

    qDebug()<<"读取一次data";
    //发送收到数据信号
    emit receiveData(ip, port, buffer);

//    //自动回复数据,可以回复的数据是以;隔开,每行可以带多个;所以这里不需要继续判断
//    if (App::DebugTcpServer) {
//        int count = App::Keys.count();
//        for (int i = 0; i < count; i++) {
//            if (App::Keys.at(i) == buffer) {
//                sendData(App::Values.at(i));
//                break;
//            }
//        }
//    }
}

void TcpClient::setIP(const QString &ip)
{
    this->ip = ip;
}

void TcpClient::setPort(int port)
{
    this->port = port;
}

void TcpClient::sendData(const QString &data)
{
    QByteArray buffer;
//    if (App::HexSendTcpServer) {
//        buffer = QUIHelper::hexStrToByteArray(data);
//    } else if (App::AsciiTcpServer) {
//        buffer = QUIHelper::asciiStrToByteArray(data);
//    } else {
//        buffer = data.toLatin1();
//    }
    buffer = data.toLatin1();
    this->write(buffer);
    emit sendData(ip, port, data);
}

TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
}


//Qt TCP多线程编程
void TcpServer::incomingConnection(qintptr handle)
{
    TcpClient *client = new TcpClient(this);

    //若套接字上发出 disconnected（）信号  则服务器调用disconnected槽函数断开连接
    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
    //若连接套接字发出sendData信号，同理服务器发出sendData信号
    connect(client, SIGNAL(sendData(QString, int, QString)), this, SIGNAL(sendData(QString, int, QString)));
    //同上
    connect(client, SIGNAL(receiveData(QString, int, QString)), this, SIGNAL(receiveData(QString, int, QString)));

    //此步相当于获得了当前连接的套接字
    client->setSocketDescriptor(handle);


    //返回连接套接字的  Ip地址和端口号
    QString ip = client->peerAddress().toString();
    int port = client->peerPort();

    client->setIP(ip);
    client->setPort(port);
    //将当前套接字放入连接列表中
    clients.append(client);
    qDebug()<<"客户端连接"<<" "<<"IP:"<<ip<<"  端口号:"<<port;
    //发送客户端连接信号
    emit clientConnected(ip, port);
}


void TcpServer::disconnected()
{
    //断开连接后从链表中移除
    //返回发送信号的套接字指针
    TcpClient *client = (TcpClient *)sender();
    //发送客户端断开连接信号
    emit clientDisconnected(client->peerAddress().toString(), client->peerPort());
    //当前客户端套接字移除列表
    clients.removeOne(client);
}


bool TcpServer::start()
{
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))

    //************************************************设置服务器侦听端口*********************************

    //bool ok = listen(QHostAddress::AnyIPv4, App::TcpListenPort);
    bool ok = listen(QHostAddress::AnyIPv4,8087);
#else
    bool ok = listen(QHostAddress::Any, App::TcpListenPort);
#endif
    qDebug()<<"开始侦听端口8087";
    return ok;
}

void TcpServer::stop()
{

    //遍历套接字列表，关闭服务器
    foreach (TcpClient *client, clients) {
        client->disconnectFromHost();
    }

    this->close();
}


//向指定IP地址和端口号 写入数据

void TcpServer::writeData(const QString &ip, int port, const QString &data)
{
    foreach (TcpClient *client, clients) {
        if (client->peerAddress().toString() == ip && client->peerPort() == port) {
            client->sendData(data);
            break;
        }
    }
}


//向连接的全部客户端  写入数据
void TcpServer::writeData(const QString &data)
{
    foreach (TcpClient *client, clients) {
        client->sendData(data);
    }
}
