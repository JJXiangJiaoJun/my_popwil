#include "tcp_app.h"
#include <QString>
#include <QObject>
#include <QDebug>


appTcpServer::appTcpServer(QWidget *parent) :
    QWidget(parent)
{
    this->initForm();
    this->initConfig();
}

appTcpServer::~appTcpServer()
{

}

void appTcpServer::initForm()
{
    isOk = false;

    //*********************************创建一个新的TCP服务器***************************************************
    tcpServer = new TcpServer(this);


    //*********************************将实例化后的 tcpserver信号函数 与 app程序的槽函数连接起来******************
    connect(tcpServer, SIGNAL(sendData(QString, int, QString)), this, SLOT(sendData(QString, int, QString)));
    connect(tcpServer, SIGNAL(receiveData(QString, int, QString)), this, SLOT(receiveData(QString, int, QString)));
    connect(tcpServer, SIGNAL(clientConnected(QString, int)), this, SLOT(clientConnected(QString, int)));
    connect(tcpServer, SIGNAL(clientDisconnected(QString, int)), this, SLOT(clientDisconnected(QString, int)));

//    timer = new QTimer(this);
//    connect(timer, SIGNAL(timeout()), this, SLOT(on_btnSend_clicked()));

//    ui->cboxInterval->addItems(App::Intervals);
//    ui->cboxData->addItems(App::Datas);
     //开始侦听端口
     if(tcpServer->start())
     {
         qDebug()<<"侦听成功";
     }
     else
     {
         qDebug()<<"侦听失败";
     }
}

void appTcpServer::initConfig()
{
//    ui->ckHexSend->setChecked(App::HexSendTcpServer);
//    connect(ui->ckHexSend, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

//    ui->ckHexReceive->setChecked(App::HexReceiveTcpServer);
//    connect(ui->ckHexReceive, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

//    ui->ckAscii->setChecked(App::AsciiTcpServer);
//    connect(ui->ckAscii, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

//    ui->ckDebug->setChecked(App::DebugTcpServer);
//    connect(ui->ckDebug, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

//    ui->ckAutoSend->setChecked(App::AutoSendTcpServer);
//    connect(ui->ckAutoSend, SIGNAL(stateChanged(int)), this, SLOT(saveConfig()));

//    ui->cboxInterval->setCurrentIndex(ui->cboxInterval->findText(QString::number(App::IntervalTcpServer)));
//    connect(ui->cboxInterval, SIGNAL(currentIndexChanged(int)), this, SLOT(saveConfig()));

//    ui->txtListenPort->setText(QString::number(App::TcpListenPort));
//    connect(ui->txtListenPort, SIGNAL(textChanged(QString)), this, SLOT(saveConfig()));

//    timer->setInterval(App::IntervalTcpServer);
//    App::AutoSendTcpServer ? timer->start() : timer->stop();
}



void appTcpServer::saveConfig()
{
//    App::HexSendTcpServer = ui->ckHexSend->isChecked();
//    App::HexReceiveTcpServer = ui->ckHexReceive->isChecked();
//    App::AsciiTcpServer = ui->ckAscii->isChecked();
//    App::DebugTcpServer = ui->ckDebug->isChecked();
//    App::AutoSendTcpServer = ui->ckAutoSend->isChecked();
//    App::IntervalTcpServer = ui->cboxInterval->currentText().toInt();
//    App::TcpListenPort = ui->txtListenPort->text().trimmed().toInt();
//    App::writeConfig();

//    timer->setInterval(App::IntervalTcpServer);
//    App::AutoSendTcpServer ? timer->start() : timer->stop();
}






void appTcpServer::append(int type, const QString &data, bool clear)
{
//    static int currentCount = 0;
//    static int maxCount = 100;

//    if (clear) {
//        ui->txtMain->clear();
//        currentCount = 0;
//        return;
//    }

//    if (currentCount >= maxCount) {
//        ui->txtMain->clear();
//        currentCount = 0;
//    }

//    if (ui->ckShow->isChecked()) {
//        return;
//    }

//    //过滤回车换行符
//    QString strData = data;
//    strData = strData.replace("\r", "");
//    strData = strData.replace("\n", "");

//    //不同类型不同颜色显示
//    QString strType;
//    if (type == 0) {
//        strType = "发送";
//        ui->txtMain->setTextColor(QColor("darkgreen"));
//    } else {
//        strType = "接收";
//        ui->txtMain->setTextColor(QColor("red"));
//    }

//    strData = QString("时间[%1] %2: %3").arg(TIMEMS).arg(strType).arg(strData);
//    ui->txtMain->append(strData);
//    currentCount++;
}

void appTcpServer::sendData(const QString &data)
{
//    if (ui->ckAll->isChecked()) {
//        tcpServer->writeData(data);
//    } else {
//        int row = ui->listWidget->currentRow();
//        if (row >= 0) {
//            QString str = ui->listWidget->item(row)->text();
//            QStringList list = str.split(":");
//            tcpServer->writeData(list.at(0), list.at(1).toInt(), data);
//        }
//    }
    tcpServer->writeData(data);
    qDebug()<<"服务器发送信息";
}

void appTcpServer::sendData(const QString &ip, int port, const QString &data)
{
     QString str = QString("[%1:%2] %3").arg(ip).arg(port).arg(data);
//    append(0, str);
     qDebug()<<str;
}

void appTcpServer::receiveData(const QString &ip, int port, const QString &data)
{
    QString str = QString("[%1:%2] %3").arg(ip).arg(port).arg(data);
//    append(1, str);
     qDebug()<<str;
}

void appTcpServer::clientConnected(const QString &ip, int port)
{
    QString str = QString("%1:%2").arg(ip).arg(port);
//    ui->listWidget->addItem(str);
//    ui->labCount->setText(QString("已连接客户端共 %1 个").arg(ui->listWidget->count()));
}

void appTcpServer::clientDisconnected(const QString &ip, int port)
{
    int row = -1;
    QString str = QString("%1:%2").arg(ip).arg(port);
//    for (int i = 0; i < ui->listWidget->count(); i++) {
//        if (ui->listWidget->item(i)->text() == str) {
//            row = i;
//            break;
//        }
//    }

//    ui->listWidget->takeItem(row);
//    ui->labCount->setText(QString("已连接客户端共 %1 个").arg(ui->listWidget->count()));
}

//void appTcpServer::on_btnListen_clicked()
//{
//    if (ui->btnListen->text() == "监听") {
//        isOk = tcpServer->start();
//        if (isOk) {
//            append(0, "监听成功");
//            ui->btnListen->setText("关闭");
//        }
//    } else {
//        isOk = false;
//        tcpServer->stop();
//        ui->btnListen->setText("监听");
//    }
//}

//void appTcpServer::on_btnSave_clicked()
//{
//    QString data = ui->txtMain->toPlainText();
//    if (data.length() <= 0) {
//        return;
//    }

//    QString fileName = QString("%1/%2.txt").arg(QUIHelper::appPath()).arg(STRDATETIME);
//    QFile file(fileName);
//    if (file.open(QFile::WriteOnly | QFile::Text)) {
//        file.write(data.toUtf8());
//        file.close();
//    }

//    on_btnClear_clicked();
//}

//void appTcpServer::on_btnClear_clicked()
//{
//    append(0, "", true);
//}

//void appTcpServer::on_btnSend_clicked()
//{
//    if (!isOk) {
//        return;
//    }

//    QString data = ui->cboxData->currentText();
//    if (data.length() <= 0) {
//        return;
//    }

//    sendData(data);
//}
