#ifndef TCP_APP_H
#define TCP_APP_H

#include "tcpserver.h"
#include <QWidget>


class appTcpServer : public QWidget
{
    Q_OBJECT

public:
    explicit appTcpServer(QWidget *parent = 0);
    ~appTcpServer();

private:

    bool isOk;
    TcpServer *tcpServer;
    QTimer *timer;

private slots:
    void initForm();
    void initConfig();
    void saveConfig();
    void append(int type, const QString &data, bool clear = false);

public slots:
    void sendData(const QString &data);

    void sendData(const QString &ip, int port, const QString &data);
    void receiveData(const QString &ip, int port, const QString &data);

    void clientConnected(const QString &ip, int port);
    void clientDisconnected(const QString &ip, int port);

private slots:
//    void on_btnListen_clicked();
//    void on_btnSave_clicked();
//    void on_btnClear_clicked();
//    void on_btnSend_clicked();
};


#endif // TCP_APP_H
