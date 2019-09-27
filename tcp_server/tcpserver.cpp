#include <QtWidgets>
#include <QHostAddress>
#include <QDebug>

#include "global_setting.h"
#include "TcpServer.h"
/**
 * @brief TcpServerAbstract::TcpServerAbstract
 * @param parent
 */
TcpServerAbstract::TcpServerAbstract(QObject *parent) : QTcpServer(parent)
{
    //连接信号槽
    connect(this,SIGNAL(newConnection()),this,SLOT(SltNewConnection()));
}
/**
 * @brief TcpServerAbstract::~TcpServerAbstract
 * 析构函数关闭服务器
 */
TcpServerAbstract::~TcpServerAbstract()
{
    if(this->isListening()) this->close();
}


/**
 * @brief TcpServerAbstract::incomingConnection
 * 每当有新连接就会调用这个函数，可以在这个函数中为每个连接开辟一个线程，实现并发服务器的功能
 * @param handle  获得的套接字
 */
void TcpServerAbstract::incomingConnection(qintptr handle)
{

    //在serverThread构造函数里面，首先设置 连接的socket套接字为 handle
    qDebug()<<"创建新线程";
    serverThread *thread = new serverThread(handle,this);

    //连接信号槽，将socket的信号与TcpServerAbstract连接起来
   connect(thread,SIGNAL(SignalNewconnection(QString,int)),this,SIGNAL(clientConnected(QString,int)));
   connect(thread,SIGNAL(SignalDisconnectToHost(QString,int,int)),this,SIGNAL(clientDisconnected(QString,int)));

    ///连接后调用的槽函数，SltConnected虚函数具体实现在子类中
   connect(thread,SIGNAL(SignalNewconnection(QString,int)),this,SLOT(SltConnected(QString,int)));

   ///用于删除断开连接的套接字，SltDisconnetctd为虚函数，具体实现在子类中
   connect(thread,SIGNAL(SignalDisconnectToHost(QString,int,int)),this,SLOT(SltDisConnected(QString,int,int)));
   ///该线程退出后，删除该线程
   connect(thread,SIGNAL(finished()),thread,SLOT(deleteLater()));
   //线程开始
   qDebug()<<"线程开始";
   thread->start();
}



/**
 * @brief TcpServerAbstract::StartListen
 * @param port  侦听端口
 * @return 开启成功返回true，否则返回false
 * 服务器开始侦听
 */
bool TcpServerAbstract::StartListen(int port)
{
    if(this->isListening()) this->close();
    bool bOk = this->listen(QHostAddress::AnyIPv4,port);
    if(!bOk)
    {
        qDebug()<<"服务器启动失败";
    }
    else
    {
        qDebug()<<"服务器启动成功，监听端口:"<<port;
    }

    return bOk;
}

void TcpServerAbstract::CloseListen()
{
    if(this->isListening()) this->close();
}


/**
  * @brief TcpServerAbstract::get_ConnectClients
  * 获得连接的套接字列表
  * @return
  */
 QList<ClientSocket *> &TcpServerAbstract::get_ConnectClients()
 {

     return this->m_clients;
 }



 /**
 * @brief TcpMsgServer::TcpMsgServer
 * @param parent
 * 构造函数，连接信号槽
 */
TcpMsgServer::TcpMsgServer(QObject *parent):
    TcpServerAbstract(parent)
{
    //启动服务器
    this->StartListen(ProtocolSet::COMMUNICATION_PORT);


    ///用于给状态栏发送消息


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
 * @brief TcpMsgServer::SltDisConnected 客户端断开连接函数
 * @param ip                            断开连接的ip
 * @param port                          断开连接的port
 * @param disconnId                     断开连接的套接字id
 */
void TcpMsgServer::SltDisConnected(const QString &ip,const int &port,const int disconnId)
{

     //获取当前的连接列表
     ///注意要返回引用!!!!!!!!!!!!!
     QList<ClientSocket *> &clients = this->get_ConnectClients();

     //从连接列表中删除断开的连接
     foreach(ClientSocket *client,clients){
         if(client->getId()==disconnId)
         {
             clients.removeOne(client);
             break;
         }
     }

}

/**
 * @brief TcpMsgServer::SltMsgToClient
 * @param type   消息类型
 * @param msg    消息内容
 * 对所有连接客户端发送消息
 */
void TcpMsgServer::SltMsgToClient(ProtocolSet::MessageTypeEnum type, QString &msg)
{

    QList<ClientSocket *> clients = this->get_ConnectClients();
    foreach (ClientSocket *client, clients) {
       #ifdef USE_DEBUG
        qDebug()<<"连接的id为 "<<client->getId();
       #endif
       client->SltSendMessage(type,msg);
    }
}

void TcpMsgServer::SendMsgToClient(ProtocolSet::MessageTypeEnum type, void *msg, const qint32 msg_len)
{
    QList<ClientSocket *> clients = this->get_ConnectClients();
    foreach (ClientSocket *client, clients) {
       #ifdef USE_DEBUG
        qDebug()<<"连接的id为 "<<client->getId();
       #endif
        client->SendMsg(type,msg,msg_len);
    }
}
