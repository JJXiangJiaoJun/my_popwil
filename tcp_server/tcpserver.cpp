#include "tcpserver.h"
#include <QtWidgets>
#include <QHostAddress>
#include <QDebug>
/**
 * @brief TcpServer::TcpServer
 * @param parent
 */
TcpServer::TcpServer(QObject *parent) : QTcpServer(parent)
{
    //连接信号槽
    connect(this,SIGNAL(newConnection()),this,SLOT(SltNewConnection()));
}
/**
 * @brief TcpServer::~TcpServer
 * 析构函数关闭服务器
 */
TcpServer::~TcpServer()
{
    if(this->isListening()) this->close();
}

///**
// * @brief TcpServer::incomingConnection
// * 每当有新连接就会调用这个函数，可以在这个函数中为每个连接开辟一个线程，实现并发服务器的功能
// * @param handle  获得的套接字
// */
//void TcpServer::incomingConnection(qintptr handle)
//{

//    //绑定套接字，并且绑定信号槽，在类的构造函数中
//    ClientSocket *client = new ClientSocket(0,handle);
//    connect(client,SIGNAL(signalDisConnected()),this,SLOT(Sltdisconnected()));


//    //返回连接套接字的  Ip地址和端口号
//    QString ip = client->m_tcpSocket->peerAddress().toString();
//    int port = client->m_tcpSocket->peerPort();

//    //设置套接字IP和端口号
//    client->setIP(ip);
//    client->setPort(port);
//    //将当前套接字放入连接列表中
//    //clients.append(client);
//    m_clients.append(client);
//    qDebug()<<"客户端连接"<<" "<<"IP:"<<ip<<"  端口号:"<<port;
//    //发送客户端连接信号
//    emit clientConnected(ip, port);


//}


/**
 * @brief TcpServer::incomingConnection
 * 每当有新连接就会调用这个函数，可以在这个函数中为每个连接开辟一个线程，实现并发服务器的功能
 * @param handle  获得的套接字
 */
void TcpServer::incomingConnection(qintptr handle)
{

    //在serverThread构造函数里面，首先设置 连接的socket套接字为 handle
    qDebug()<<"创建新线程";
    serverThread *thread = new serverThread(handle,this);

    //连接信号槽，将socket的信号与tcpserver连接起来
   connect(thread,SIGNAL(SignalNewconnection(QString,int)),this,SIGNAL(clientConnected(QString,int)));
   connect(thread,SIGNAL(SignalDisconnectToHost(QString,int)),this,SIGNAL(clientDisconnected(QString,int)));
   connect(thread,SIGNAL(SignalDisconnectToHost(QString,int)),this,SLOT(Sltdisconnected(QString,int)));
   ///该线程退出后，删除该线程
   connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
   //线程开始
   qDebug()<<"线程开始";
   thread->start();
}


///**
// * @brief TcpServer::Sltdisconnected
// * 客户端断开连接调用槽函数
// */
//void TcpServer::Sltdisconnected()
//{
//     ClientSocket *client = (ClientSocket *)sender();
//     TcpSocket *tcp_socket = client->m_tcpSocket;

//     emit clientDisconnected(tcp_socket->peerAddress().toString(),tcp_socket->peerPort());
//     m_clients.removeOne(client);
//}
/**
 * @brief TcpServer::Sltdisconnected
 * @param ip
 * @param port
 */
void TcpServer::Sltdisconnected(QString &ip,int &port)
{
     emit clientDisconnected(ip,port);
}

/**
 * @brief TcpServer::StartListen
 * @param port  侦听端口
 * @return 开启成功返回true，否则返回false
 * 服务器开始侦听
 */
bool TcpServer::StartListen(int port)
{
    if(this->isListening()) this->close();
    bool bOk = this->listen(QHostAddress::AnyIPv4,port);
    if(!bOk)
    {
        qDebug()<<"服务器启动失败";
    }
    else
    {
        qDebug()<<endl;
        qDebug()<<"服务器启动成功，监听端口:"<<port;
    }
    return bOk;
}

void TcpServer::CloseListen()
{
    if(this->isListening()) this->close();
}


/**
  * @brief TcpServer::get_ConnectClients
  * 获得连接的套接字列表
  * @return
  */
 QList<ClientSocket *> &TcpServer::get_ConnectClients()
 {

     return this->m_clients;
 }



 /**
 * @brief TcpMsgServer::TcpMsgServer
 * @param parent
 * 构造函数，连接信号槽
 */
TcpMsgServer::TcpMsgServer(QObject *parent):
    TcpServer(parent)
{
    //启动服务器
    this->StartListen(ProtocolSet::COMMUNICATION_PORT);


    connect(this,SIGNAL(clientConnected(QString,int)),this,SIGNAL(newconnect_client(QString,int)));
    connect(this,SIGNAL(clientDisconnected(QString,int)),this,SIGNAL(disconnect_client(QString,int)));
    connect(this,SIGNAL(clientDisconnected(QString,int)),this,SLOT(SltDisConnected()));
    connect(this,SIGNAL(clientConnected(QString,int)),this,SLOT(SltConnected()));

}

/**
 * @brief TcpMsgServer::~TcpMsgServer
 * 析构函数关闭服务器
 */
TcpMsgServer::~TcpMsgServer()
{
  qDebug()<<"tcp 服务器关闭";
  QList<ClientSocket *> &clients = this->get_ConnectClients();

  foreach(ClientSocket *client,clients){
     clients.removeOne(client);
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
    qDebug()<<"服务器与客户端建立连接";
}

/**
 * @brief TcpMsgServer::SltDisConnected
 * 客户端断开连接
 */
void TcpMsgServer::SltDisConnected()
{
    qDebug()<<"服务器与客户端断开连接";
}
/**
 * @brief TcpMsgServer::SltMsgToClient
 * @param type   消息类型
 * @param msg    消息内容
 * 对所有连接客户端发送消息
 */
void TcpMsgServer::SltMsgToClient(ProtocolSet::MessageType type, QString &msg)
{

    QList<ClientSocket *> clients = this->get_ConnectClients();
    foreach (ClientSocket *client, clients) {
       client->SltSendMessage(type,msg);
    }
}

