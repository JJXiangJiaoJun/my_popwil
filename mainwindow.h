#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDockWidget>
#include <QMainWindow>
#include <QGuiApplication>
#include <QWidget>
#include <QDialog>
#include <QDateTime>
#include <QTimer>
#include <QLabel>
#include <QScrollbar>
#include <QTime>
#include <QLineEdit>
#include <QTextBrowser>
#include <QString>

#include "qdoublebufferedqueue.h"
#include "randomwalk.h"
#include "performancetimer.h"
#include "new_experiment.h"
#include "restricted_para.h"
#include "shake_table_para.h"
#include "TcpServer.h"
#include "mytitlebar.h"
#include "mybasewindow.h"
#include "qchartviewer.h"
#include "chartdir.h"
#include "chartdata.h"
#include "myhelper.h"
#include "global_setting.h"
#include "globaldata.h"
#include "mychartviewer.h"



namespace Ui {
class MainWindow;
}



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();


private:
    Ui::MainWindow *ui;

    //主窗口的两个DockWidget控件
    QDockWidget *msgDock;
    QDockWidget *statusDock;
    //Qcharviewer绘图控件
    QFrame       *leftFrame;
    //QChartViewer *m_ChartViewer;        // QChartViewer control
    MyChartViewer *dPlot;
    QChartViewer *m_previewChart;


    long msCount;
    long msStartCount;
    double elapsedTime;
    PerformanceTimer *timer;
    QLabel *tcpstatus;

    //*********************TCP server********************

    TcpMsgServer *m_tcpmsgserver;
    //*********************StatusDock*******************
    QTime *m_totalRunningTime;
    QTime *m_MeasureTime;
    QTime *m_ReleaveTime;

    QLineEdit *TotalRunningTime_LineEdit;
    QLineEdit *MeasureTime_LineEdit;
    QLineEdit *ReleaveTime_LineEdit;
    QLineEdit *HintMsg_LineEdit;

    QLineEdit *WaveForm_LineEdit;
    QLineEdit *Ampli_LineEdit;
    QLineEdit *Frequency_LineEdit;
    QLineEdit *ControlMethod_LineEdit;
    QLineEdit *CurPos_LineEdit;
    QLineEdit *CurVel_LineEdit;
    QLineEdit *CurAcc_LineEdit;

    QTextBrowser *PosPeakValue_TextBrowser;
    QTextBrowser *VelocityValue_TextBrowser;
    QTextBrowser *SampleRate1_TextBrowser;
    QTextBrowser *SampleRate2_TextBrowser;
    QTextBrowser *AccRMS1_TextBrowser;
    QTextBrowser *AccRMS2_TextBrowser;
    QTextBrowser *DriverPeakValue_TextBrowser;
    QTextBrowser *Order_TextBrowser;


    QTimer *statusUpdateTimer;
private slots:
    void on_Start_btn_clicked();

    void on_User_1_triggered();

    void on_Setting_control_para_triggered();

    void slotFuction();                             //10ms多媒体定时器
    void on_action_Exit_triggered();
    void on_action_New_triggered();



    //服务器连接函数
    void tcpsever_connect(const QString &ip);
    void tcpsever_disconnect(const QString &ip);
    void on_Setting_channel_para_triggered();
    void on_Setting_unit_triggered();
    void on_Setting_confine_para_triggered();

    void on_Setting_target_triggered();


    void on_Stop_btn_clicked();

    void StatusUpdateTimerSlot();

    void ExperimentParamChangeSlot();

    void on_action_print_triggered();

    void on_GenerateReport_Action_triggered();

    void on_RealPosRadioButton_clicked();

    void on_RealVelRadioButton_clicked();

    void on_RealAccRadioButton_clicked();

    void on_PreviewWave_PushButton_clicked();

    void on_tool_bar_Pointer_triggered();

    void on_tool_bar_ZoomIn_triggered();

    void on_tool_bar_ZoomOut_triggered();

    void on_tool_bar_Save_File_triggered();

    void on_LoadWave_PushButton_clicked();

private:
    void StatusDockInit();
    void MessageDockInit();
    void GlobalVariableInit();
    void WavePreview(QString title);
};




class MyMainWindow:public MyBaseWindow
{
    Q_OBJECT
public:
    MyMainWindow(QWidget *parent=0);
    MainWindow *m_MainWindow;
};


#endif // MAINWINDOW_H
