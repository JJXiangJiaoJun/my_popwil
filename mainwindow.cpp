#include <QScreen>
#include <QWidget>
#include <QLayout>
#include <QAction>
#include <QButtonGroup>
#include <QPushButton>
#include <QApplication>
#include <math.h>
#include <vector>
#include <sstream>
#include <QProcess>
#include <QIcon>
#include <QComboBox>
#include <QFileDialog>
#include <QLineEdit>
#include <algorithm>
#include <QFile>
#include <QStringList>
#include <QFileInfo>

#include "QTime"
#include "qdatetime.h"
#include "qmessagebox.h"
#include "performancetimer.h"
#include "qdesktopservices.h"
#include "qmath.h"
#include "qdebug.h"
#include "channel_param.h"
#include "set_measurement_unit.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qdebug.h"
#include "loginscreen.h"
#include "control_param.h"

QString send_data = "This is Qt Tcp Server\r\n";



using namespace std;

#define PERFORMANCEINTERVAL 10
#define STATUSUPDATEINTERVAL 200

/*******************************************
 * @brief MyMainWindow::MyMainWindow
 * @param parent
 */
MyMainWindow::MyMainWindow(QWidget *parent):
    MyBaseWindow(parent)
{
    //设置窗口大小
    this->resize(QSize(1100,900));
    //可以重新设置标题栏的图标和内容

    //新建下方的主窗口
    m_MainWindow = new MainWindow(this);
    //添加到布局中
    addWindows(m_MainWindow);
    m_titleBar->setTitleWidth(this->width());
}

/**
 * @brief MainWindow::MainWindow
 * @param parent
 */
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //隐藏标题栏
    this->setWindowFlags(Qt::FramelessWindowHint);
    //初始化自定义标题栏
    msgDock = ui->Message_dock;
    statusDock = ui->Status_dock;
    ui->Message_dock->setMinimumHeight(250);

    GlobalVariableInit();
    StatusDockInit();
    MessageDockInit();

 //*********************************************************************************************
    resize(QSize(1000,800));

//    QVBoxLayout *RealTimeTabVBoxLayout = new QVBoxLayout(ui->RealTimeTab);
//    //add to layout
//    RealTimeTabVBoxLayout->addWidget(m_ChartViewer);
//    RealTimeTabVBoxLayout->addWidget(m_ViewPortControl);
    dPlot = new MyChartViewer(ui->RealTimeTab);
    dPlot->start();
    connect(ui->tool_bar_Save_File, SIGNAL(triggered(bool)), dPlot,SLOT(onSave(bool)));
//    RealTimeTabVBoxLayout->addLayout(ui->RealTime_HBoxLayout);
//    RealTimeTabVBoxLayout->addWidget(dPlot);
    //ui->RealTimeTab->setLayout(RealTimeTabVBoxLayout);
/****************************************************************************************/
    //*****preview Charter
    QVBoxLayout *PreviewTabVBoxLayout = new QVBoxLayout(ui->PreviewTab);
    m_previewChart = new QChartViewer(ui->PreviewTab);
    PreviewTabVBoxLayout->addLayout(ui->Preview_HBoxLayout);
    PreviewTabVBoxLayout->addWidget(m_previewChart);
    ui->DynamicPlotTabWidget->setCurrentIndex(0);
    WavePreview("sinewave");
    ui->PreviewTab->setLayout(PreviewTabVBoxLayout);

    connect(ui->PreviewPosRadioButton,SIGNAL(clicked(bool)),this,SLOT(on_PreviewWave_PushButton_clicked()));
    connect(ui->PreviewVelRadioButton,SIGNAL(clicked(bool)),this,SLOT(on_PreviewWave_PushButton_clicked()));
    connect(ui->PreviewAccRadioButton,SIGNAL(clicked(bool)),this,SLOT(on_PreviewWave_PushButton_clicked()));
    connect(ui->PreviewFRadioButton,SIGNAL(clicked(bool)),this,SLOT(on_PreviewWave_PushButton_clicked()));
//*********************************参数初始化************************************************************

    timer = new PerformanceTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(slotFuction()));
    timer->start(PERFORMANCEINTERVAL);  //10为毫秒

    msCount=0;
    elapsedTime = 0.0;
 //*************************************************创建TCP服务器通信*******************
    QStatusBar *statubar = this->statusBar();
    tcpstatus = new QLabel(statubar);
    statubar->addPermanentWidget(tcpstatus);
    statubar->showMessage(QString("已开放端口%1,等待连接").arg(ProtocolSet::COMMUNICATION_PORT),3000);
    tcpstatus->setText("未连接到控制器");
    g_ConnectedClientCount = 0;



     m_tcpmsgserver = new TcpMsgServer(this);
     connect(m_tcpmsgserver,SIGNAL(clientConnected(QString,int)),this,SLOT(tcpsever_connect(QString)));
     connect(m_tcpmsgserver,SIGNAL(clientDisconnected(QString,int)),this,SLOT(tcpsever_disconnect(QString)));

     //GlobalData::g_TcpMsgServer = m_tcpmsgserver;
     g_TcpMsgServer = m_tcpmsgserver;

  //************************************************两个DockWidget的设置***********************************

     //只能停留在右方
     msgDock->setAllowedAreas(Qt::RightDockWidgetArea|Qt::LeftDockWidgetArea);
     statusDock->setAllowedAreas(Qt::RightDockWidgetArea|Qt::LeftDockWidgetArea);



}

MainWindow::~MainWindow()
{
    delete ui;
}



//******************************************************************************************************************
//10ms多媒体定时器
//100ms绘图
void MainWindow::slotFuction()
{

    bool pos_valid_flag,vel_valid_flag,acc_valid_flag,f_valid_flag;
    bool valid_flag;
    ChartDataType cur_pos,ref_pos,cur_vel,ref_vel,cur_acc,ref_acc,cur_f,ref_f;
    DataPacket packet;
    ChartDataType cur_data=0.0,cur_ref=0.0;
    msCount      += PERFORMANCEINTERVAL;
    msStartCount += PERFORMANCEINTERVAL;
    //elapsedTime  += (double)PERFORMANCEINTERVAL/1000.0;


    //从控制器传来的数据
    //如位移、速度等等 类型为double型
    //series0 为实际波形，series1 为参考波形
    //packet.series0  = 0.124*sin(2*PI*5.0*k_t*20.0/1000);

    if(!g_IsRunning){
        pos_valid_flag = g_PosData.GetCurData(cur_pos);
        ref_pos = 0.0;
        vel_valid_flag = g_VelData.GetCurData(cur_vel);
        ref_vel = 0.0;
        acc_valid_flag = g_AccData.GetCurData(cur_acc);
        ref_acc = 0.0;
        f_valid_flag = g_FData.GetCurData(cur_f);
        ref_f = 0.0;
    }
    else
    {
        pos_valid_flag = g_PosData.GetRunningData(cur_pos,ref_pos);
        vel_valid_flag = g_VelData.GetRunningData(cur_vel,ref_vel);
        acc_valid_flag = g_AccData.GetRunningData(cur_acc,ref_acc);
        f_valid_flag = g_FData.GetRunningData(cur_f,ref_f);
    }

    if(pos_valid_flag)
        g_CurPos = cur_pos;
    if(vel_valid_flag)
        g_CurVel = cur_vel;
    if(acc_valid_flag)
        g_CurAcc = cur_acc;
    if(f_valid_flag)
        g_CurF  = cur_f;
    switch (g_DisplayType) {
    case ChartDisplayTypeEnum::PlotPos:
        cur_data = cur_pos;
        cur_ref = ref_pos;
        valid_flag = pos_valid_flag;
        break;
    case ChartDisplayTypeEnum::PlotVel:
        cur_data = cur_vel;
        cur_ref = ref_vel;
        valid_flag = vel_valid_flag;
        break;
    case ChartDisplayTypeEnum::PlotAcc:
        cur_data = cur_acc;
        cur_ref = ref_acc;
        valid_flag = acc_valid_flag;
        break;
    case ChartDisplayTypeEnum::PlotF:
        cur_data = cur_f;
        cur_ref = ref_f;
        valid_flag = f_valid_flag;
    default:
        break;
    }

    //更新最大值
    if(g_IsRunning&&pos_valid_flag)
    {
        g_PosPeakValue = std::max(g_PosPeakValue,cur_pos);
    }
    if(g_IsRunning&&acc_valid_flag)
    {
        g_AccPeakValue = std::max(g_AccPeakValue,cur_acc);
    }

    //不是正在运行
    //或者读取数据有效则加入绘图
    //否则不进行绘图
    if(!g_IsRunning||valid_flag){
        elapsedTime  += (double)PERFORMANCEINTERVAL/1000.0;
        packet.series0 = cur_data;
        packet.series1 =cur_ref;
        packet.elapsedTime = elapsedTime;

        g_buffer.put(packet);
    }


}

/**
 * @brief MainWindow::WavePreview
 * @param title
 * 预览绘图
 */
void MainWindow::WavePreview(QString title)
{
    //最多预览前10000个点
    int previewCnt = g_PosRefArray.dataCnt<10000?g_PosRefArray.dataCnt:10000;
    double index[10000];

    for(int i=0;i<previewCnt;i++)
        index[i] = i*g_PosRefArray.samplePeroid;
    //m_previewChart->setGeometry(15, 50, 640, 500);
    XYChart *c = new XYChart(650,480); //画布大小，包括图标以及标题
    c->setPlotArea(30, 30, c->getWidth() - 41, c->getHeight() - 60, c->linearGradientColor(0, 30, 0,
       c->getHeight() - 50, 0xf0f6ff, 0xa0c0ff), -1, 0xffffff, 0xffffff);
    c->addTitle(title.toLatin1(), "simsun.ttc", 18);
    c->setBackground(0xccccff,0x000000);
    if(ui->PreviewPosRadioButton->isChecked())
    {
        c->addLineLayer(DoubleArray(g_PosRefArray.buffer,previewCnt));//添加y轴数据
        c->addText(5, 5, "S/mm", "timesbi.ttf", 11, 0xff0000);
    }
    if(ui->PreviewVelRadioButton->isChecked())
    {
        c->addLineLayer(DoubleArray(g_VelRefArray.buffer,previewCnt));//添加y轴数据
        c->addText(5, 5, "V/mm/s", "timesbi.ttf", 11, 0xff0000);
    }
    if(ui->PreviewAccRadioButton->isChecked())
    {
        c->addLineLayer(DoubleArray(g_AccRefArray.buffer,previewCnt));//添加y轴数据
        c->addText(5, 5, "A/g", "timesbi.ttf", 11, 0xff0000);
    }
    if(ui->PreviewFRadioButton->isChecked())
    {
        c->addLineLayer(DoubleArray(g_FRefArray.buffer,previewCnt));//添加y轴数据
        c->addText(5, 5, "F/N", "timesbi.ttf", 11, 0xff0000);
    }

    c->xAxis()->setLabels(DoubleArray(index,previewCnt));//添加x轴数据，有点类似C里的指针操作
    c->addText(630, 450, "t/s", "timesbi.ttf", 11, 0xff0000);
    c->xAxis()->setLabelStep(500);//x轴绘图间隔

    m_previewChart->setChart(c);
    delete c;
    m_previewChart->show();

}

void MainWindow::GlobalVariableInit()
{

    //位移峰值
     g_PosPeakValue = 0.0;
    //加速度峰值
     g_AccPeakValue = 0.0;

     g_IsRunning  = false;

     g_ExperimentParam.waveform = WaveFormEnum::SineWave;
     g_ExperimentParam.amplitude = 0.1;
     g_ExperimentParam.frequency = 5.0;
}


void MainWindow::MessageDockInit()
{
    WaveForm_LineEdit = ui->WaveForm_LineEdit;
    Ampli_LineEdit = ui->Ampli_LineEdit;
    Frequency_LineEdit = ui->Frequency_LineEdit;
    ControlMethod_LineEdit = ui->ControlMethod_LineEdit;
    CurPos_LineEdit = ui->CurPos_LineEdit;
    CurVel_LineEdit = ui->CurVel_LineEdit;
    CurAcc_LineEdit = ui->CurAcc_LineEdit;

}

void MainWindow::StatusDockInit()
{
    //初始化LineEdit
    TotalRunningTime_LineEdit = ui->TotalRuningTime_LineEdit;
    MeasureTime_LineEdit = ui->MeasureTime_LineEdit;
    ReleaveTime_LineEdit = ui->ReleaveTime_LineEdit;
    HintMsg_LineEdit = ui->HintMsg_LineEdit;

    //初始化TextBrowser
    PosPeakValue_TextBrowser = ui->PosPeakValue_TextBrowser;
    PosPeakValue_TextBrowser->setPlaceholderText("请开始试验");
    VelocityValue_TextBrowser = ui->VelocityPeakValue_TextBrowser;
    SampleRate1_TextBrowser = ui->SampleRate1_TextBrowser;
    SampleRate2_TextBrowser = ui->SampleRate2_TextBrowser;
    AccRMS1_TextBrowser = ui->AccRMS1_TextBrowser;
    AccRMS2_TextBrowser = ui->AccRMS2_TextBrowser;
    DriverPeakValue_TextBrowser = ui->DriverPeakValue_TextBrowser;
    Order_TextBrowser = ui->Order_TextBrowser;

    //初始化更新定时器
    statusUpdateTimer = new QTimer();
    connect(statusUpdateTimer,SIGNAL(timeout()),this,SLOT(StatusUpdateTimerSlot()));

    //初始化QTime类
    m_totalRunningTime = new QTime(0,0,0,0);
    m_MeasureTime = new QTime(0,0,0,0);
    m_ReleaveTime = new QTime(0,10,0,0);

    //初始化全局参数
    //GlobalData::g_AccPeakValue = 0.0;
    g_AccPeakValue = 0.0;
   // GlobalData::g_PosPeakValue = 0.0;
    g_PosPeakValue = 0.0;

    TotalRunningTime_LineEdit->setReadOnly(true);
    QString curRunningTime = m_totalRunningTime->toString("hh:mm:ss");
    TotalRunningTime_LineEdit->setText(QString(curRunningTime));
    TotalRunningTime_LineEdit->displayText();

    MeasureTime_LineEdit->setReadOnly(true);
    QString curMeasureTime = m_MeasureTime->toString("hh:mm:ss");
    MeasureTime_LineEdit->setText(QString(curMeasureTime));
    MeasureTime_LineEdit->displayText();

    ReleaveTime_LineEdit = ui->ReleaveTime_LineEdit;
    QString curReleaveTime = m_ReleaveTime->toString("hh:mm:ss");
    ReleaveTime_LineEdit->setText(QString(curReleaveTime));
    ReleaveTime_LineEdit->displayText();

    HintMsg_LineEdit->setPlaceholderText("未连接请开始试验");

//    GlobalData::g_AccPeakValue = 1.0755;
//    GlobalData::g_PosPeakValue = 2.1024;

//    g_AccPeakValue = 0.0755;
//    g_PosPeakValue = 0.1204;

    //初始化TextBrowser
    //QString curPosPeakValue = QString::number(GlobalData::g_AccPeakValue,'f',4);
    QString curPosPeakValue = QString::number(g_AccPeakValue,'f',4);
    PosPeakValue_TextBrowser->setText(curPosPeakValue);


    QString curSampleRate1 = QString::number(100);
    QString curSampleRate2 = QString::number(1000);
    SampleRate1_TextBrowser->setText(curSampleRate1);
    SampleRate2_TextBrowser->setText(curSampleRate2);

    QString curVelocityValue = QString::number(0.0293,'f',4);
    VelocityValue_TextBrowser->setText(curVelocityValue);

    //QString curAccRMS1 = QString::number(GlobalData::g_AccPeakValue,'f',4);
    QString curAccRMS1 = QString::number(g_AccPeakValue,'f',4);
    AccRMS1_TextBrowser->setText(curAccRMS1);
    //QString curAccRMS2 = QString::number(GlobalData::g_AccPeakValue,'f',4);
    QString curAccRMS2 = QString::number(g_PosPeakValue,'f',4);
    AccRMS2_TextBrowser->setText(curAccRMS2);

    QString curDriverPeakValue = QString::number(0.004168,'f',4);
    DriverPeakValue_TextBrowser->setText(curDriverPeakValue);

    QString curOrder = QString("正常量级");
    Order_TextBrowser->setText(curOrder);

    //GlobalData::g_IsRunning = false;
    g_IsRunning = false;
    statusUpdateTimer->start(STATUSUPDATEINTERVAL);

}


void MainWindow::StatusUpdateTimerSlot()
{
    //update the runing time
    *m_totalRunningTime = m_totalRunningTime->addMSecs(STATUSUPDATEINTERVAL);
    QString curRunningTime = m_totalRunningTime->toString("hh:mm:ss");
    TotalRunningTime_LineEdit->setText(curRunningTime);
    TotalRunningTime_LineEdit->displayText();

    if(g_IsRunning == true)
    //if(GlobalData::g_IsRunning == true)
    {
        *m_MeasureTime = m_MeasureTime->addMSecs(STATUSUPDATEINTERVAL);
        *m_ReleaveTime = m_ReleaveTime->addMSecs(-STATUSUPDATEINTERVAL);
        //QString curPosPeakValue = QString::number(GlobalData::g_AccPeakValue,'f',4);
//        g_PosPeakValue = g_AccPeakValue + 0.1274;
        QString curPosPeakValue = QString::number(g_PosPeakValue,'f',4);
        PosPeakValue_TextBrowser->setText(curPosPeakValue);
        //QString curAccRMS1 = QString::number(GlobalData::g_AccPeakValue,'f',4);
        QString curAccRMS1 = QString::number(g_AccPeakValue,'f',4);
        AccRMS1_TextBrowser->setText(curAccRMS1);
       // QString curAccRMS2 = QString::number(GlobalData::g_AccPeakValue,'f',4);
        QString curAccRMS2 = QString::number(g_AccPeakValue+0.0037,'f',4);
        AccRMS2_TextBrowser->setText(curAccRMS2);
//        QString curMeasureTime = m_totalRunningTime->toString("hh:mm:ss");
//        MeasureTime_LineEdit->setText(curMeasureTime);
//        MeasureTime_LineEdit->displayText();
    }
    QString curMeasureTime = m_MeasureTime->toString("hh:mm:ss");
    MeasureTime_LineEdit->setText(curMeasureTime);
    MeasureTime_LineEdit->displayText();

    QString curReleaveTime = m_ReleaveTime->toString("hh:mm:ss");
    ReleaveTime_LineEdit->setText(QString(curReleaveTime));
    ReleaveTime_LineEdit->displayText();


    WaveForm_LineEdit->setText(g_WaveFormStringList[g_ExperimentParam.waveform]);
    Ampli_LineEdit->setText(QString::number(g_ExperimentParam.amplitude));
    Frequency_LineEdit->setText(QString::number(g_ExperimentParam.frequency));
    ControlMethod_LineEdit->setText(g_ControlMethodStringList[g_ControlMethod]);
    CurPos_LineEdit->setText(QString::number(g_CurPos));
    CurVel_LineEdit->setText(QString::number(g_CurVel));
    CurAcc_LineEdit->setText(QString::number(g_CurAcc));


    //qDebug()<<curTime;
}


/**
 * @brief MainWindow::ExperimentParamChangeSlot
 * 改变试验显示信息
 */
void MainWindow::ExperimentParamChangeSlot()
{
    QLineEdit *AmpliLineEdit = ui->Ampli_LineEdit;
    QLineEdit *FrequncyLineEdit = ui->Frequency_LineEdit;
    QString AmpliStr = QString::number(g_ExperimentParam.amplitude,'f',1);
    QString FrequncyStr = QString::number(g_ExperimentParam.frequency,'f',1);

    AmpliLineEdit->setText(AmpliStr);
    FrequncyLineEdit->setText(FrequncyStr);

}


/**
 * @brief MainWindow::on_Start_btn_clicked
 *开始按钮按下
 */
void MainWindow::on_Start_btn_clicked()
{

    //向控制器发送起始帧
    qint16 cmd = CommandEnum::Start;
    g_TcpMsgServer->SendMsgToClient(ProtocolSet::COMMAND,&cmd,sizeof(cmd));
    //设置状态为开始运行并清空全局变量
    //g_IsRunning = true;
    g_PosPeakValue = 0.0;
    g_AccPeakValue = 0.0;
    HintMsg_LineEdit->setPlaceholderText("试验正在进行.....");


}
/**
 * @brief MainWindow::on_Stop_btn_clicked
 */
void MainWindow::on_Stop_btn_clicked()
{

    //向控制器停止帧
    qint16 cmd = CommandEnum::Stop;
    g_TcpMsgServer->SendMsgToClient(ProtocolSet::COMMAND,&cmd,sizeof(cmd));
    g_IsRunning = false;
    //清零计数器
    m_MeasureTime->setHMS(0,0,0);
    HintMsg_LineEdit->setPlaceholderText("试验停止");
}

//点击菜单栏用户登录按钮
void MainWindow::on_User_1_triggered()
{
    loginscreen *log = new loginscreen ;
    log->show();
}


//点击菜单栏控制参数设置
void MainWindow::on_Setting_control_para_triggered()
{
    control_param *control_window = new control_param;
    //ui->Message_dock->show();
    control_window->show();
}


//**************************点击菜单栏的退出按钮*****************************
void MainWindow::on_action_Exit_triggered()
{

    int ret=QMessageBox::information(this,"提示信息","请确实是否退出操作界面？",
                                     QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
    switch(ret)
    {
    case QMessageBox::Ok:
        qDebug()<<"closing window";
        this->close();
        break;
    case QMessageBox::Cancel:
        qDebug()<<"will not close";
        break;
    default:
        break;
    }

}


//******************************按下菜单栏----新建实验按钮-----
void MainWindow::on_action_New_triggered()
{
    new_experiment *new_exp = new new_experiment;
    connect(new_exp,&new_experiment::ExperimentParamChangeSingal,this,&ExperimentParamChangeSlot);
    new_exp->exec();
}


void MainWindow::tcpsever_connect(const QString &ip)
{
    QStatusBar *statubar = this->statusBar();
    statubar->showMessage("新的控制器连接 IP:"+ip,3000);
    g_ConnectedClientCount++;
    QString sta = QString("当前控制器连接个数: %1").arg(g_ConnectedClientCount);
    tcpstatus->setText(sta);

    qDebug()<<"回调tcpsever_connect";

}

void MainWindow::tcpsever_disconnect(const QString &ip)
{
    QStatusBar *statubar = this->statusBar();
    //若出现错误
    g_ConnectedClientCount--;
    statubar->showMessage("控制器断开连接",3000);
    QString sta;
    if(g_ConnectedClientCount==0)
        sta.sprintf("未连接到控制器");
    else
        sta.sprintf("当前控制器连接个数: %d",g_ConnectedClientCount);
    tcpstatus->setText(sta);
    qDebug()<<"回调tcpsever_disconnect";
}

void MainWindow::on_Setting_channel_para_triggered()
{
    channel_param *new_channel =new channel_param;
    new_channel->show();
}
//按下了菜单栏“工程单位”
void MainWindow::on_Setting_unit_triggered()
{
    set_measurement_unit *measure_unit = new set_measurement_unit;
    measure_unit->show();
}


void MainWindow::on_Setting_confine_para_triggered()
{
    restricted_para *restrict_unit = new restricted_para;
    restrict_unit->show();
}

void MainWindow::on_Setting_target_triggered()
{
    shake_table_para *shake_table_unit = new shake_table_para;
    shake_table_unit->show();
}


/**
 * @brief MainWindow::on_action_print_triggered
 */
void MainWindow::on_action_print_triggered()
{

}

/**
 * @brief MainWindow::on_GenerateReport_Action_triggered
 * 打印结果
 */
void MainWindow::on_GenerateReport_Action_triggered()
{

      QString fileName = QFileDialog::getSaveFileName(this,"保存文件","",
                                                      "文本文件(*.txt);;CSV文件(*.csv)");
      if(!ReportGenerator::GenerateCSVReport(fileName,QString(",")))
      {
          QMessageBox::warning(this,"提示信息",QString("生成报告失败"),
                                               QMessageBox::Ok,QMessageBox::Ok);
          return;
      }
      QFileInfo fileInfo(fileName);

//    QFile *report = new QFile("report.txt");
//    if(!report->open(QIODevice::WriteOnly|QIODevice::Text))
//    {
//        qDebug()<<"Open file failed";
//    }

//    QTextStream out(report);
//    out.setCodec("UTF-8");
//    QString ExperimentIDStr = "试验编号";
//    QString ExperimentID = QString::number(g_ExperimentID);

//    QString WaveFormStr = "波形";
//    QString WaveForm = "正弦波";

//    QString ControlStrategyStr = "控制方法";
//    QString ControlStrategy = "PID";

//    QString ControlParamStr = "控制参数";
//    QString P = "P\t0.25";
//    QString I = "I\t0.25";
//    QString D = "D\t0.25";

//    QString ExperimentParamStr = "实验参数";
//    QString Amplifier = QString("幅值\t%1%2").arg("25.0").arg("mm");
//    QString Frequncy = QString("频率\t%1%2").arg("1.0").arg("Hz");
//    QString Phase = QString("相位\t%1%2").arg("0.0").arg("");


//    out <<ExperimentIDStr << endl;
//    out <<ExperimentID << endl;
//    out <<endl<<endl;


//    out << WaveFormStr <<endl;
//    out << WaveForm <<endl;
//    out <<endl<<endl;

//    out << ControlStrategyStr <<endl;
//    out << ControlStrategy << endl;
//    out <<endl<<endl;

//    out << ControlParamStr << endl;
//    out << P << endl;
//    out << I << endl;
//    out << D << endl;
//    out <<endl<<endl;

//    out << ExperimentParamStr << endl;
//    out << Amplifier << endl;
//    out << Frequncy << endl;
//    out << Phase << endl;
//    out <<endl<<endl;

//    report->close();

    int ret=QMessageBox::information(this,"提示信息",QString("成功生成csv文件:%1").arg(fileInfo.fileName()),
                                     QMessageBox::Ok,QMessageBox::Ok);

}

/**
 * @brief MainWindow::on_RealPosRadioButton_clicked
 * 实时绘图位移显示RadioButton按下
 */
void MainWindow::on_RealPosRadioButton_clicked()
{
    g_DisplayType = ChartDisplayTypeEnum::PlotPos;
}



void MainWindow::on_RealVelRadioButton_clicked()
{
    g_DisplayType = ChartDisplayTypeEnum::PlotVel;
}



void MainWindow::on_RealAccRadioButton_clicked()
{
    g_DisplayType = ChartDisplayTypeEnum::PlotAcc;
}

void MainWindow::on_RealFRadioButton_clicked()
{
    g_DisplayType = ChartDisplayTypeEnum::PlotF;
}


void MainWindow::on_PreviewWave_PushButton_clicked()
{
    QString title;
    ui->DynamicPlotTabWidget->setCurrentIndex(1);

    QStringList waveModeString;
    waveModeString<<"Sine Wave"<<"Sine Sweep Wave"<<"Random Wave"<<"Triangle Wave";
    title = waveModeString[g_ExperimentParam.waveform];
    WavePreview(title);
}

void MainWindow::on_tool_bar_Pointer_triggered()
{
    dPlot->onMouseUsageChanged(Chart::MouseUsageScroll);
}


void MainWindow::on_tool_bar_ZoomIn_triggered()
{
    dPlot->onMouseUsageChanged(Chart::MouseUsageZoomIn);
}



void MainWindow::on_tool_bar_ZoomOut_triggered()
{
    dPlot->onMouseUsageChanged(Chart::MouseUsageZoomOut);
}

void MainWindow::on_tool_bar_Save_File_triggered()
{
    dPlot->onSave(true);
}

/**
 * @brief MainWindow::on_LoadWave_PushButton_clicked
 * 载入当前波形按下
 */
void MainWindow::on_LoadWave_PushButton_clicked()
{
    //判断当前是否有参考波形
    if(g_PosRefArray.dataCnt==0||g_VelRefArray.dataCnt==0||g_AccRefArray.dataCnt==0)
    {
        QMessageBox::warning(this," 警告","未生成参考波形",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
        return;
    }
    //使用服务器将参考波形发送至控制器中
    //发送正弦波形参数
    g_TcpMsgServer->SendMsgToClient(ProtocolSet::SineWaveParam,&g_ExperimentParam,sizeof(ExperimentParamStruct));

//    //参考位移
//    g_TcpMsgServer->SendMsgToClient(ProtocolSet::PosRefData,&g_PosRefArray,g_PosRefArray.dataCnt*sizeof(double));
//    //参考速度
//    g_TcpMsgServer->SendMsgToClient(ProtocolSet::VelRefData,&g_VelRefArray,g_VelRefArray.dataCnt*sizeof(double));
//    //参考加速度
//    g_TcpMsgServer->SendMsgToClient(ProtocolSet::AccRefData,&g_VelRefArray,g_AccRefArray.dataCnt*sizeof(double));
}

