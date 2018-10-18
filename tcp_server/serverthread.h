#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QObject>
#include <QThread>
#include <QtNetwork>
#include <QDebug>

#include "clientsocket.h"


class serverThread : public QThread
{
    Q_OBJECT
public:
    serverThread(qintptr socketDesc,QObject *parent=0);
    ~serverThread();
    void run();
private:
    qintptr socketDescriptor;

public:
    qintptr getSocketDescriptor();
    ClientSocket *getConnectSocket();
signals:
    void SignalDisconnectToHost(QString ip,int port);
    void SignalNewconnection(QString ip,int port);
    void SignalSendMsgToHost(ProtocolSet::MessageType &type,QString &data);
private slots:
    void SltDisconnectToHost();
    void SltNewconnection();

private:
    ClientSocket *m_socket;
};

#endif // SERVERTHREAD_H
