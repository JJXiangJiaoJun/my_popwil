#include "serverthread.h"
#include "QDebug"
/**
 * @brief serverThread::serverThread
 * @param socketDescriptor   该连接线程的套接字描述符
 * @param parent
 */
serverThread::serverThread(qintptr socketDesc, QObject *parent):
    QThread(parent),socketDescriptor(socketDesc)
{

    //新建套接字
    m_socket = new ClientSocket(this,socketDescriptor);

    //将新连接的套接字加入服务器的连接中
    TcpServerAbstract *connect_server = (TcpServerAbstract*) parent;
    connect_server->get_ConnectClients().append(m_socket);
}

/**
 * @brief serverThread::~serverThread
 * 删除申请的套接字
 */
serverThread::~serverThread()
{
    m_socket->Close();
    delete m_socket;

}


/**
 * @brief serverThread::getSocketDescriptor
 * @return 返回当前连接的套接字
 */
qintptr serverThread::getSocketDescriptor()
{
    return this->socketDescriptor;
}

ClientSocket* serverThread::getConnectSocket()
{
    return this->m_socket;
}

/**
 * @brief serverThread::run
 * 线程运行函数，处理逻辑主体,主要是连接线程信号槽与，套接字发送信号槽
 */
void serverThread::run()
{

    qDebug()<<"run()----------------------------------";

    qDebug()<<"创建新的套接字连接";

    QString cur_ip=m_socket->getIP();
    int cur_port =m_socket->getPort();


    connect(m_socket,SIGNAL(signalConnected(QString,int)),this,SLOT(SltNewconnection()));


    connect(m_socket,SIGNAL(signalDisConnected(QString,int,int)),this,SLOT(SltDisconnectToHost()));

    //发送消息信号
    connect(this,SIGNAL(SignalSendMsgToHost(ProtocolSet::MessageTypeEnum&,QString&)),\
            m_socket,SLOT(SltSendMessage(ProtocolSet::MessageTypeEnum&,QString&)));

     //断开连接的话退出该线程
   connect(m_socket,SIGNAL(signalConnected(QString,int)),this,SLOT(quit()));

     //发送新连接信号
   emit SignalNewconnection(cur_ip,cur_port);
    //线程开始运行
    exec();
}

/**
 * @brief serverThread::SltDisconnectToHost
 */
void serverThread::SltDisconnectToHost()
{
   m_socket->Disconnect();
   emit SignalDisconnectToHost(m_socket->getIP(),m_socket->getPort(),m_socket->getId());

}

/**
 * @brief serverThread::SltNewconnection
 */
void serverThread::SltNewconnection()
{
    emit SignalNewconnection(m_socket->getIP(),m_socket->getPort());
}

