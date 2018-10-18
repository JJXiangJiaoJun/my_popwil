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
//    m_socket = new ClientSocket(0,socketDescriptor);
//    //连接信号槽  客户端连接，断开信号槽
//    connect(m_socket,SIGNAL(signalDisConnected(QString,int)),this,SIGNAL(SignalDisconnectToHost(QString,int)));
//    connect(m_socket,SIGNAL(signalConnected(QString,int)),this,SIGNAL(SignalNewconnection(QString,int)));

//    connect(m_socket,SIGNAL(signalConnected(QString,int)),this,SLOT(SltNewconnection()));
//    connect(m_socket,SIGNAL(signalDisConnected(QString,int)),this,SLOT(SltDisconnectToHost()));


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
    m_socket = new ClientSocket(this,socketDescriptor);
    qDebug()<<"创建新的套接字连接";
    QString cur_ip=m_socket->getIP();
    int cur_port =m_socket->getPort();
    //connect(m_socket,SIGNAL(signalDisConnected(QString,int)),this,SIGNAL(SignalDisconnectToHost(QString,int)));
    //connect(m_socket,SIGNAL(signalConnected(QString,int)),this,SIGNAL(SignalNewconnection(QString,int)));

    connect(m_socket,SIGNAL(signalConnected(QString,int)),this,SLOT(SltNewconnection()));
     qDebug()<<" connect(m_socket,SIGNAL(signalConnected(QString,int)),this,SLOT(SltNewconnection()))";

    connect(m_socket,SIGNAL(signalDisConnected(QString,int)),this,SLOT(SltDisconnectToHost()));
    qDebug()<<"connect(m_socket,SIGNAL(signalDisConnected(QString,int)),this,SLOT(SltDisconnectToHost()))";
    //发送消息信号
    connect(this,SIGNAL(SignalSendMsgToHost(ProtocolSet::MessageType&,QString&)),\
            m_socket,SLOT(SltSendMessage(ProtocolSet::MessageType&,QString&)));
   qDebug()<<"connect(this,SIGNAL(SignalSendMsgToHost(ProtocolSet::MessageType&,QString&)),\n\
             m_socket,SLOT(SltSendMessage(ProtocolSet::MessageType&,QString&)));";
     //断开连接的话退出该线程
   connect(m_socket,SIGNAL(signalConnected(QString,int)),this,SLOT(quit()));
    qDebug()<<" connect(m_socket,SIGNAL(signalConnected(QString,int)),this,SLOT(quit()));";
     //发送新连接信号
     emit SignalNewconnection(cur_ip,cur_port);
   qDebug()<<"发送连接信号";
    //线程开始运行
    exec();
}

/**
 * @brief serverThread::SltDisconnectToHost
 */
void serverThread::SltDisconnectToHost()
{
   m_socket->Disconnect();
   emit SignalDisconnectToHost(m_socket->getIP(),m_socket->getPort());

}

/**
 * @brief serverThread::SltNewconnection
 */
void serverThread::SltNewconnection()
{
    emit SignalNewconnection(m_socket->getIP(),m_socket->getPort());
}

