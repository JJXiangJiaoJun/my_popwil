#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QObject>
#include <QThread>
#include <QtNetwork>
#include <QDebug>

#include "clientsocket.h"
#include "TcpServer.h"




class serverThread : public QThread
{
    Q_OBJECT
public:
    serverThread(qintptr socketDesc,QObject *parent=0);
    ~serverThread();

    qintptr         getSocketDescriptor();
    ClientSocket    *getConnectSocket();
    void run();
private:
    //当前连接的套接字描述符
    qintptr         socketDescriptor;
    //当前连接的Socket指针对象
    ClientSocket    *m_socket;

signals:
    void SignalDisconnectToHost(QString ip,int port,int disconnId);
    void SignalNewconnection(QString ip,int port);
    void SignalSendMsgToHost(ProtocolSet::MessageTypeEnum &type,QString &data);

private slots:
    void SltDisconnectToHost();
    void SltNewconnection();

};

#endif // SERVERTHREAD_H
