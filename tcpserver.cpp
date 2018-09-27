#include "tcpserver.h"
#include <QtWidgets>
#include <QHostAddress>
#include <QDebug>
TcpClient::TcpClient(QObject *parent) :  QTcpSocket(parent)
{
//    ip = "192.168.1.3";
//    port = 8087;

//    connect(this, SIGNAL(connected()), this, SLOT(connected()));
//    connect(this, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(deleteLater()));
//    connect(this, SIGNAL(disconnected()), this, SLOT(deleteLater()));
//    connect(this, SIGNAL(disconnected()), this, SLOT(disconnected()));
//    connect(this, SIGNAL(readyRead()), this, SLOT(readData()));
}

void TcpClient::connected()
{
//    emit sendData(ip, port, "客户端上线");
//    qDebug()<<"客户端上线";
}

void TcpClient::disconnected()
{
//    emit sendData(ip, port, "客户端下线");
//    qDebug()<<"客户端下线";
}

void TcpClient::readData()
{
//    //TCP字符流连接
//    QByteArray data = this->readAll();
//    if (data.length() <= 0) {
//        return;
//    }

//    QString buffer;
////    if (App::HexReceiveTcpServer) {
////        buffer = QUIHelper::byteArrayToHexStr(data);
////    } else if (App::AsciiTcpServer) {
////        buffer = QUIHelper::byteArrayToAsciiStr(data);
////    } else {
////        buffer = QString(data);
////    }
//    buffer = QString(data);

//    qDebug()<<"读取一次data";
//    //发送收到数据信号
//    emit receiveData(ip, port, buffer);

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
//    this->ip = ip;
}

void TcpClient::setPort(int port)
{
//    this->port = port;
}

void TcpClient::sendData(const QString &data)
{
//    QByteArray buffer;
////    if (App::HexSendTcpServer) {
////        buffer = QUIHelper::hexStrToByteArray(data);
////    } else if (App::AsciiTcpServer) {
////        buffer = QUIHelper::asciiStrToByteArray(data);
////    } else {
////        buffer = data.toLatin1();
////    }
//    buffer = data.toLatin1();
//    qDebug()<<"直接发送的数据为"<<buffer;
//    this->write(buffer);
//    emit sendData(ip, port, data);
}

void TcpClient::sendData(const QString &data,ProtocolSet::MessageType type)
{
//    QByteArray buffer;

//    ProtocolSet msg;
//    //buffer为构造好的数据包
//    buffer = msg.send_Msg(type,data);

//    //发送数据包
//    this->write(buffer);
//    emit sendData(ip, port, data);
}



TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{

}


/**
 * @brief TcpServer::incomingConnection
 * 每当有新连接就会调用这个函数，可以在这个函数中为每个连接开辟一个线程，实现并发服务器的功能
 * @param handle  获得的套接字
 */
void TcpServer::incomingConnection(qintptr handle)
{
//    TcpClient *client = new TcpClient(this);

//    //若套接字上发出 disconnected（）信号  则服务器调用disconnected槽函数断开连接
//    connect(client, SIGNAL(disconnected()), this, SLOT(disconnected()));
//    //若连接套接字发出sendData信号，同理服务器发出sendData信号
//    connect(client, SIGNAL(sendData(QString, int, QString)), this, SIGNAL(sendData(QString, int, QString)));
//    //同上
//    connect(client, SIGNAL(receiveData(QString, int, QString)), this, SIGNAL(receiveData(QString, int, QString)));

//    //此步相当于获得了当前连接的套接字
//    client->setSocketDescriptor(handle);

    //绑定套接字，并且绑定信号槽，在类的构造函数中
    ClientSocket *client = new ClientSocket(this,handle);
    connect(client,SIGNAL(signalDisConnected()),this,SLOT(Sltdisconnected()));


    //返回连接套接字的  Ip地址和端口号
    QString ip = client->m_tcpSocket->peerAddress().toString();
    int port = client->m_tcpSocket->peerPort();

    //设置套接字IP和端口号
    client->setIP(ip);
    client->setPort(port);
    //将当前套接字放入连接列表中
    //clients.append(client);
    m_clients.append(client);
    qDebug()<<"客户端连接"<<" "<<"IP:"<<ip<<"  端口号:"<<port;
    //发送客户端连接信号
    emit clientConnected(ip, port);



//    //绑定套接字，并且绑定信号槽，在类的构造函数中
//    ClientSocket *client = new ClientSocket(this,handle);


}
/**
 * @brief TcpServer::Sltdisconnected
 */
void TcpServer::Sltdisconnected()
{
     ClientSocket *client = (ClientSocket *)sender();
     TcpSocket *tcp_socket = client->m_tcpSocket;

     emit clientDisconnected(tcp_socket->peerAddress().toString(),tcp_socket->peerPort());
     m_clients.removeOne(client);
}

void TcpServer::disconnected()
{
//    //断开连接后从链表中移除
//    //返回发送信号的套接字指针
//    TcpClient *client = (TcpClient *)sender();
//    //发送客户端断开连接信号
//    emit clientDisconnected(client->peerAddress().toString(), client->peerPort());
//    //当前客户端套接字移除列表
//    clients.removeOne(client);
}


bool TcpServer::start()
{
#if (QT_VERSION > QT_VERSION_CHECK(5,0,0))

    //************************************************设置服务器侦听端口*********************************

    bool ok = listen(QHostAddress::AnyIPv4,ProtocolSet::COMMUNICATION_PORT);
#else
    bool ok = listen(QHostAddress::Any, App::TcpListenPort);
#endif
    qDebug()<<"开始侦听端口："<<ProtocolSet::COMMUNICATION_PORT;
    return ok;
}

void TcpServer::stop()
{

//    //遍历套接字列表，关闭服务器
//    foreach (TcpClient *client, clients) {
//        client->disconnectFromHost();
//    }

//    this->close();
}


//向指定IP地址和端口号 写入数据

void TcpServer::writeData(const QString &ip, int port, const QString &data)
{
//    foreach (TcpClient *client, clients) {
//        if (client->peerAddress().toString() == ip && client->peerPort() == port) {
//            client->sendData(data);
//            break;
//        }
//    }
}


//向连接的全部客户端  写入数据
void TcpServer::writeData(const QString &data)
{
//    foreach (TcpClient *client, clients) {
//        client->sendData(data);
//    }
}


void TcpServer::writeData(const QString &data,ProtocolSet::MessageType type)
{
//    foreach (TcpClient *client, clients) {
//        client->sendData(data,type);
//    }
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/**
 * @brief TCPServer::TCPServer
 * @param parent
 */
TCPServer::TCPServer(QObject *parent):
    QObject(parent)
{
    m_tcpServer = new TcpServer(this);

    connect(m_tcpServer,SIGNAL(newConnection()),this,SLOT(SltNewConnection()));
}
/**
 * @brief TCPServer::~TCPServer
 * //析构函数，关闭服务器
 */
TCPServer::~TCPServer()
{
    if(m_tcpServer->isListening())  m_tcpServer->close();
}
/**
 * @brief TCPServer::StartListen
 * 启动服务器，监听端口
 * @param port
 * @return
 */
bool TCPServer::StartListen(int port)
{
    if(m_tcpServer->isListening()) m_tcpServer->close();
    bool bOk = m_tcpServer->listen(QHostAddress::Any,port);
    if(!bOk)
    {
        qDebug()<<"服务器启动失败";
    }
    else
    {
        qDebug()<<"服务器启动成功，监听端口:"<<ProtocolSet::COMMUNICATION_PORT;
    }
    return bOk;
}
/**
 * @brief TCPServer::CloseListen
 * 关闭服务器
 */
void TCPServer::CloseListen()
{
    m_tcpServer->close();
}

TcpMsgServer::TcpMsgServer(QObject *parent):
    TCPServer(parent)
{
    //启动服务器
    this->StartListen(ProtocolSet::COMMUNICATION_PORT);

    connect(this->m_tcpServer, SIGNAL(clientConnected(QString, int)), this, SIGNAL(newconnect_client(QString,int)));
    connect(this->m_tcpServer, SIGNAL(clientDisconnected(QString, int)), this, SIGNAL(disconnect_client(QString, int)));
}


TcpMsgServer::~TcpMsgServer()
{
  qDebug()<<"tcp 服务器关闭";
  foreach (ClientSocket *client, this->m_tcpServer->m_clients) {
      this->m_tcpServer->m_clients.removeOne(client);
      client->Close();
  }
}

/**
 * @brief TcpMsgServer::SltNewConnection
 * 有新的客户端连接进来
 */
void TcpMsgServer::SltNewConnection()
{

}

/**
 * @brief TcpMsgServer::SltConnected
 */
void TcpMsgServer::SltConnected()
{

}


void TcpMsgServer::SltDisConnected()
{

}

void TcpMsgServer::SltMsgToClient(ProtocolSet::MessageType type, QString &msg)
{
    foreach (ClientSocket *client, this->m_tcpServer->m_clients) {
       client->SltSendMessage(type,msg);
    }
}

