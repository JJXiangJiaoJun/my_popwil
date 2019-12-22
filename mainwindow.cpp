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
#define PI   3.141592657
#define PERFORMANCEINTERVAL 10
#define PERIOD 1000
#define PAINT_PERIOD 100
#define STATUSUPDATEINTERVAL 1000

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


 //*********************************************************************************************
    resize(QSize(1000,800));
    // Pointer push button
    QPushButton *pointerPB = new QPushButton();
    // Zoom In push button
    QPushButton *zoomInPB = new QPushButton();
    // Zoom Out push button
    QPushButton *zoomOutPB = new QPushButton();
    //****************Save as Picture
    connect(ui->tool_bar_Save_File, SIGNAL(triggered(bool)), SLOT(onSave(bool)));

    pointerPB->setVisible(false);
    zoomInPB->setVisible(false);
    zoomOutPB->setVisible(false);


    // The Pointer/Zoom In/Zoom Out buttons form a button group
    QButtonGroup *mouseUsage = new QButtonGroup();

    mouseUsage->addButton(pointerPB, Chart::MouseUsageScroll);
    mouseUsage->addButton(zoomInPB, Chart::MouseUsageZoomIn);
    mouseUsage->addButton(zoomOutPB, Chart::MouseUsageZoomOut);


    connect(mouseUsage, SIGNAL(buttonPressed(int)), SLOT(onMouseUsageChanged(int)));

    connect(ui->tool_bar_Pointer,SIGNAL(triggered()),pointerPB,SLOT(click()));
    connect(ui->tool_bar_ZoomIn,SIGNAL(triggered()),zoomInPB,SLOT(click()));
    connect(ui->tool_bar_ZoomOut,SIGNAL(triggered()),zoomOutPB,SLOT(click()));


    //动态绘图区域

    QVBoxLayout *RealTimeTabVBoxLayout = new QVBoxLayout(ui->RealTimeTab);


/****************************绘制实时曲线Qcharviewer控件************************************/
    // Chart Viewer
    //m_ChartViewer = new QChartViewer(dynamic_polt_area);
    m_ChartViewer = new QChartViewer(ui->RealTimeTab);

    //m_ChartViewer->setGeometry(10, 25, 640, 480);

    // Enable mouse wheel zooming by setting the zoom ratio to 1.1 per wheel event

    // Configure the initial viewport
    connect(m_ChartViewer, SIGNAL(viewPortChanged()), SLOT(onViewPortChanged()));
    connect(m_ChartViewer, SIGNAL(mouseMovePlotArea(QMouseEvent*)),
    SLOT(onMouseMovePlotArea(QMouseEvent*)));
    m_ViewPortControl = new QViewPortControl(ui->RealTimeTab);

    //m_ViewPortControl->setGeometry(10, 480, 640, 80);
    m_ViewPortControl->setViewer(m_ChartViewer);

    //add to layout
    RealTimeTabVBoxLayout->addWidget(m_ChartViewer);
    RealTimeTabVBoxLayout->addWidget(m_ViewPortControl);
/****************************************************************************************/
    //*****preview Charter
    QVBoxLayout *PreviewTabVBoxLayout = new QVBoxLayout(ui->PreviewTab);
    m_previewChart = new QChartViewer(ui->PreviewTab);
    PreviewTabVBoxLayout->addWidget(m_previewChart);
//*********************************参数初始化************************************************************

    //
    // Initialize member variables
    //
    m_currentIndex = 0;

    // Initially, auto-move the track line to make it follow the data series
    trackLineEndPos = 0;
    trackLineIsAtEnd = true;

    // Initially set the mouse to drag to scroll mode.
    pointerPB->click();

    // Enable mouse wheel zooming by setting the zoom ratio to 1.1 per wheel event
    m_ChartViewer->setMouseWheelZoomRatio(1.1);

    // Configure the initial viewport
    //配置初始视野
    m_ChartViewer->setViewPortWidth(initialVisibleRange / (double)initialFullRange);

    // Start the random data generator
    //dataSource = new RandomWalk(OnData, this);
    //dataSource->start();

    // Set up the chart update timer
    m_ChartUpdateTimer = new QTimer(this);
    connect(m_ChartUpdateTimer, SIGNAL(timeout()), SLOT(onChartUpdateTimer()));

    // The chart update rate is set to PAINT_PERIOD ms
    //绘图定时器，100ms绘制一次
    m_ChartUpdateTimer->start(PAINT_PERIOD);

    timer = new PerformanceTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(slotFuction()));
    timer->start(PERFORMANCEINTERVAL);  //10为毫秒

    msCount=0;
    elapsedTime = 0.0;
 //*************************************************创建TCP服务器通信*******************
    QStatusBar *statubar = this->statusBar();
    tcpstatus = new QLabel(statubar);
    statubar->addPermanentWidget(tcpstatus);
    statubar->showMessage("已开放端口8087,等待连接",3000);
    tcpstatus->setText("未连接到控制器");
    TcpServerAbstract_count = 0;



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
    delete m_ChartViewer->getChart();
}

void MainWindow::onViewPortChanged()
{
    // Update the chart if necessary
    if (m_ChartViewer->needUpdateChart())
        drawChart(m_ChartViewer);
     // Update the full chart
    drawFullChart(m_ViewPortControl);
}

//
// **************************************************Get data from the queue, update the viewport and update the chart display if necessary.
//
void MainWindow::onChartUpdateTimer()
{
    QChartViewer *viewer = m_ChartViewer;

    // Enables auto scroll if the viewport is showing the latest data before the update
    bool autoScroll = (m_currentIndex > 0) && (0.001 + viewer->getValueAtViewPort("x",
        viewer->getViewPortLeft() + viewer->getViewPortWidth()) >= m_timeStamps[m_currentIndex - 1]);


    //从队列中获取数据，并把他们加入到 data arrays中（绘图的数组)
    // Get new data from the queue and append them to the data arrays
    //
    int count;
    DataPacket *packets;

    //通过此步操作 ， 读取缓冲区和存储缓冲器交换
    if ((count = buffer.get(&packets)) <= 0)
        return;
    //qDebug()<<"双缓冲队列中数据个数:"<<count;
    //上边这步非常有意思，利用二级指针作为函数的参数
    //经过上边这个操作之后，buffer里面的数据就转到了packets指针中，长度为count

    //m_currentIndex为绘图数组当前存取索引 , 如果空间不足，将删除旧数据
    // if data arrays have insufficient space, we need to remove some old data.
    if (m_currentIndex + count >= sampleSize)
    {
        // For safety, we check if the queue contains too much data than the entire data arrays. If
        // this is the case, we only use the latest data to completely fill the data arrays.
        if (count > sampleSize)
        {
            packets += count - sampleSize;
            count = sampleSize;
        }

        // Remove oldest data to leave space for new data. To avoid frequent removal, we ensure at
        // least 5% empty space available after removal.

        //至少移动 5%的空间，防止频繁移动
        //保存当前索引
        int originalIndex = m_currentIndex;

        //获取95%的索引
        m_currentIndex = sampleSize * 95 / 100 - 1;
        //还是大于，则往前减小索引
        if (m_currentIndex > sampleSize - count)
            m_currentIndex = sampleSize - count;
        //将所有数据往前移动一段距离，大概是5%
        for (int i = 0; i < m_currentIndex; ++i)
        {
            int srcIndex = i + originalIndex - m_currentIndex;
            m_timeStamps[i] = m_timeStamps[srcIndex];
            m_dataSeriesA[i] = m_dataSeriesA[srcIndex];
            m_dataSeriesB[i] = m_dataSeriesB[srcIndex];
        }
    }
    //qDebug()<<"显示缓冲区的数据个数："<<m_currentIndex;
    // Append the data from the queue to the data arrays
    //将packet类型数组中数据取出来，放到显示数组中去
    for (int n = 0; n < count; ++n)
    {
        m_timeStamps[m_currentIndex] = packets[n].elapsedTime;
        m_dataSeriesA[m_currentIndex] = packets[n].series0;
        m_dataSeriesB[m_currentIndex] = packets[n].series1;
        //qDebug()<<m_timeStamps[m_currentIndex]<<m_dataSeriesA[m_currentIndex]<<m_dataSeriesB[m_currentIndex];
        ++m_currentIndex;
    }
    //此时要显示的数据全部存储在那三个数组中
    //qDebug()<<"显示缓冲区的数据个数："<<m_currentIndex;

    //这个是下面进度条全局图
    // As we added more data, we may need to update the full range of the viewport.
    //

    double startDate = m_timeStamps[0];
    double endDate = m_timeStamps[m_currentIndex - 1];

    // Use the initialFullRange (which is 60 seconds in this demo) if this is sufficient.
    double duration = endDate - startDate;
    if (duration < initialFullRange)
        endDate = startDate + initialFullRange;

    // Update the new full data range to include the latest data
    bool axisScaleHasChanged = viewer->updateFullRangeH("x", startDate, endDate,
        Chart::KeepVisibleRange);

    if (autoScroll)
    {
        // Scroll the viewport if necessary to display the latest data
        double viewPortEndPos = viewer->getViewPortAtValue("x", m_timeStamps[m_currentIndex - 1]);
        if (viewPortEndPos > viewer->getViewPortLeft() + viewer->getViewPortWidth())
        {
            viewer->setViewPortLeft(viewPortEndPos - viewer->getViewPortWidth());
            axisScaleHasChanged = true;
        }
    }

    // Set the zoom in limit as a ratio to the full range
    viewer->setZoomInWidthLimit(zoomInLimit / (viewer->getValueAtViewPort("x", 1) -
        viewer->getValueAtViewPort("x", 0)));

    // Trigger the viewPortChanged event. Updates the chart if the axis scale has changed
    // (scrolling or zooming) or if new data are added to the existing axis scale.
    viewer->updateViewPort(axisScaleHasChanged || (duration < initialFullRange), false);
}



//******************************************************************************************************************
//10ms多媒体定时器
//100ms绘图
void MainWindow::slotFuction()
{

    bool pos_valid_flag,vel_valid_flag,acc_valid_flag;
    bool valid_flag;
    ChartDataType cur_pos,ref_pos,cur_vel,ref_vel,cur_acc,ref_acc;
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
    }
    else
    {
        pos_valid_flag = g_PosData.GetRunningData(cur_pos,ref_pos);
        vel_valid_flag = g_VelData.GetRunningData(cur_vel,ref_vel);
        acc_valid_flag = g_AccData.GetRunningData(cur_acc,ref_acc);
    }


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
    default:
        break;
    }

    //更新最大值
    if(g_IsRunning)
    {
        g_AccPeakValue = std::max(g_AccPeakValue,packet.series0);
    }

    //不是正在运行
    //或者读取数据有效则加入绘图
    //否则不进行绘图
    if(!g_IsRunning||valid_flag){
        elapsedTime  += (double)PERFORMANCEINTERVAL/1000.0;
        packet.series0 = cur_data;
        packet.series1 =cur_ref;
        packet.elapsedTime = elapsedTime;
        buffer.put(packet);
    }


}





//
//************************************************************************** Draw chart
//
void MainWindow::drawChart(QChartViewer *viewer)
{
    // Get the start date and end date that are visible on the chart.
    double viewPortStartDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft());
    double viewPortEndDate = viewer->getValueAtViewPort("x", viewer->getViewPortLeft() +
        viewer->getViewPortWidth());

    // Extract the part of the data arrays that are visible.
    DoubleArray viewPortTimeStamps;
    DoubleArray viewPortDataSeriesA;
    DoubleArray viewPortDataSeriesB;
    /*
     * m_timeStamps,m_dataSeriesA,m_dataSeriesB:从双缓冲区中读取的所有数据
    */
    //qDebug()<<m_currentIndex;

    if (m_currentIndex > 0)
    {
        // Get the array indexes that corresponds to the visible start and end dates
        int startIndex = (int)floor(Chart::bSearch(DoubleArray(m_timeStamps, m_currentIndex), viewPortStartDate));
        int endIndex = (int)ceil(Chart::bSearch(DoubleArray(m_timeStamps, m_currentIndex), viewPortEndDate));
        int noOfPoints = endIndex - startIndex + 1;

        // Extract the visible data
        if (m_timeStamps[endIndex] >= viewPortStartDate)
        {
            viewPortTimeStamps = DoubleArray(m_timeStamps+ startIndex, noOfPoints);
            viewPortDataSeriesA = DoubleArray(m_dataSeriesA + startIndex, noOfPoints);
            viewPortDataSeriesB = DoubleArray(m_dataSeriesB + startIndex, noOfPoints);
        }

        // Keep track of the latest available data at chart plotting time
        trackLineEndPos = m_timeStamps[m_currentIndex - 1];
    }

    //
    // At this stage, we have extracted the visible data. We can use those data to plot the chart.
    //

    //================================================================================
    // Configure overall chart appearance.
    //================================================================================

    // Create an XYChart object of size 640 x 350 pixels
    XYChart *c = new XYChart(640, 450);

    // Set the plotarea at (20, 30) with width 41 pixels less than chart width, and height 50 pixels
    // less than chart height. Use a vertical gradient from light blue (f0f6ff) to sky blue (a0c0ff)
    // as background. Set border to transparent and grid lines to white (ffffff).
    c->setPlotArea(20, 30, c->getWidth() - 41, c->getHeight() - 50, c->linearGradientColor(0, 30, 0,
        c->getHeight() - 50, 0xf0f6ff, 0xa0c0ff), -1, Chart::Transparent, 0xffffff, 0xffffff);

     // As the data can lie outside the plotarea in a zoomed chart, we need enable clipping.
    c->setClipping();

    // Add a title to the chart using 18pt Arial font
    //c->addTitle("   Multithreading Real-Time Chart", "arial.ttf", 18);
    c->addTitle("实时曲线", "simsun.ttc", 18);

    // Add a legend box at (55, 25) using horizontal layout. Use 10pt Arial Bold as font. Set the
    // background and border color to transparent and use line style legend key.
    LegendBox *b = c->addLegend(55, 25, false, "arialbd.ttf", 10);
    b->setBackground(Chart::Transparent);
    b->setLineStyleKey();

    // Set the x and y axis stems to transparent and the label font to 10pt Arial
    c->xAxis()->setColors(Chart::Transparent);
    c->yAxis()->setColors(Chart::Transparent);
    c->xAxis()->setLabelStyle("arial.ttf", 10);
    c->yAxis()->setLabelStyle("arial.ttf", 10, 0x336699);

    // Set the y-axis tick length to 0 to disable the tick and put the labels closer to the axis.
    c->yAxis()->setTickLength(0);

    // Add axis title using 10pt Arial Bold Italic font
    c->yAxis()->setTitle("velocity", "arialbd.ttf", 10);

    // Configure the y-axis label to be inside the plot area and above the horizontal grid lines
    c->yAxis()->setLabelGap(-1);
    c->yAxis()->setLabelAlignment(1);
    c->yAxis()->setMargin(20);

    // Configure the x-axis labels to be to the left of the vertical grid lines
    //设置X轴标签与竖直网格线的位置关系，0表示正下方，1表示向右偏
    c->xAxis()->setLabelAlignment(0);

    //================================================================================
    // Add data to chart
    //================================================================================

    //
    // In this example, we represent the data by lines. You may modify the code below to use other
    // representations (areas, scatter plot, etc).
    //

    // Add a line layer for the lines, using a line width of 2 pixels
    LineLayer *layer = c->addLineLayer();
    layer->setLineWidth(1);
    layer->setFastLineMode();

    // Now we add the 2 data series to the line layer with red (ff0000) and green (00cc00) colors
    layer->setXData(viewPortTimeStamps);
    layer->addDataSet(viewPortDataSeriesA, 0xff0000, "line1");
    layer->addDataSet(viewPortDataSeriesB, 0x00cc00, "line2");

    //================================================================================
    // Configure axis scale and labelling
    //================================================================================

    // Set the x-axis as a date/time axis with the scale according to the view port x range.
    if (m_currentIndex > 0)
        c->xAxis()->setDateScale(viewPortStartDate, viewPortEndDate);

    // For the automatic axis labels, set the minimum spacing to 75/30 pixels for the x/y axis.
    c->xAxis()->setTickDensity(75);
    c->yAxis()->setTickDensity(30);

    // We use "hh:nn:ss" as the axis label format.
    c->xAxis()->setLabelFormat("{value|hh:nn:ss}");

    // We make sure the tick increment must be at least 1 second.
    //c->xAxis()->setMinTickInc(1);

    // Set the auto-scale margin to 0.05, and the zero affinity to 0.6
    c->yAxis()->setAutoScale(0.05, 0.05, 0.6);

    //================================================================================
    // Output the chart
    //================================================================================

    // We need to update the track line too. If the mouse is moving on the chart (eg. if
    // the user drags the mouse on the chart to scroll it), the track line will be updated
    // in the MouseMovePlotArea event. Otherwise, we need to update the track line here.
    if (!viewer->isInMouseMoveEvent())
        trackLineLabel(c, trackLineIsAtEnd ? c->getWidth() : viewer->getPlotAreaMouseX());

    // Set the chart image to the QChartViewer
    delete viewer->getChart();
    viewer->setChart(c);
}

//
//*************************************************** Draw track cursor when mouse is moving over plotarea
//
void MainWindow::onMouseMovePlotArea(QMouseEvent *)
{
    double trackLinePos = trackLineLabel((XYChart *)m_ChartViewer->getChart(),
        m_ChartViewer->getPlotAreaMouseX());
    trackLineIsAtEnd = (m_currentIndex <= 0) || (trackLinePos == trackLineEndPos);

    m_ChartViewer->updateDisplay();
}



//
//******************************************************* Draw the track line with data point labels
//
double MainWindow::trackLineLabel(XYChart *c, int mouseX)
{
    // Clear the current dynamic layer and get the DrawArea object to draw on it.
    DrawArea *d = c->initDynamicLayer();

    // The plot area object
    PlotArea *plotArea = c->getPlotArea();

    // Get the data x-value that is nearest to the mouse, and find its pixel coordinate.
    double xValue = c->getNearestXValue(mouseX);
    int xCoor = c->getXCoor(xValue);
    if (xCoor < plotArea->getLeftX())
        return xValue;

    // Draw a vertical track line at the x-position
    d->vline(plotArea->getTopY(), plotArea->getBottomY(), xCoor, 0x888888);

    // Draw a label on the x-axis to show the track line position.
    ostringstream xlabel;
    xlabel << "<*font,bgColor=000000*> " << c->xAxis()->getFormattedLabel(xValue + 0.00499,
        "hh:nn:ss.ff") << " <*/font*>";
    TTFText *t = d->text(xlabel.str().c_str(), "arialbd.ttf", 10);

    // Restrict the x-pixel position of the label to make sure it stays inside the chart image.
    int xLabelPos = max(0, min(xCoor - t->getWidth() / 2, c->getWidth() - t->getWidth()));
    t->draw(xLabelPos, plotArea->getBottomY() + 2, 0xffffff);
    t->destroy();

    // Iterate through all layers to draw the data labels
    for (int i = 0; i < c->getLayerCount(); ++i) {
        Layer *layer = c->getLayerByZ(i);

        // The data array index of the x-value
        int xIndex = layer->getXIndexOf(xValue);

        // Iterate through all the data sets in the layer
        for (int j = 0; j < layer->getDataSetCount(); ++j)
        {
            DataSet *dataSet = layer->getDataSetByZ(j);
            const char *dataSetName = dataSet->getDataName();

            // Get the color, name and position of the data label
            int color = dataSet->getDataColor();
            int yCoor = c->getYCoor(dataSet->getPosition(xIndex), dataSet->getUseYAxis());

            // Draw a track dot with a label next to it for visible data points in the plot area
            if ((yCoor >= plotArea->getTopY()) && (yCoor <= plotArea->getBottomY()) && (color !=
                Chart::Transparent) && dataSetName && *dataSetName)
            {
                d->circle(xCoor, yCoor, 4, 4, color, color);

                ostringstream label;
                label << "<*font,bgColor=" << hex << color << "*> "
                    << c->formatValue(dataSet->getValue(xIndex), "{value|P4}") << " <*font*>";
                t = d->text(label.str().c_str(), "arialbd.ttf", 10);

                // Draw the label on the right side of the dot if the mouse is on the left side the
                // chart, and vice versa. This ensures the label will not go outside the chart image.
                if (xCoor <= (plotArea->getLeftX() + plotArea->getRightX()) / 2)
                    t->draw(xCoor + 6, yCoor, 0xffffff, Chart::Left);
                else
                    t->draw(xCoor - 6, yCoor, 0xffffff, Chart::Right);

                t->destroy();
            }
        }
    }

    return xValue;
}

//
//  ***********************************************************The Pointer, Zoom In or Zoom out button is pressed
//
void MainWindow::onMouseUsageChanged(int mouseUsage)
{
    m_ChartViewer->setMouseUsage(mouseUsage);
}


//
// **************************************************Draw the full thumbnail chart and display it in the given QViewPortControl
//
void MainWindow::drawFullChart(QViewPortControl *vpc)
{
    // Create an XYChart object of size 640 x 60 pixels
    XYChart *c = new XYChart(640, 60);

    // Set the plotarea with the same horizontal position as that in the main chart for alignment.
    c->setPlotArea(20, 0, c->getWidth() - 41, c->getHeight() - 1, 0xc0d8ff, -1, 0xcccccc,
        Chart::Transparent, 0xffffff);

    // Set the x axis stem to transparent and the label font to 10pt Arial
    c->xAxis()->setColors(Chart::Transparent);
    c->xAxis()->setLabelStyle("Arial", 10);

    // Put the x-axis labels inside the plot area by setting a negative label gap. Use
    // setLabelAlignment to put the label at the right side of the tick.
    c->xAxis()->setLabelGap(-1);
    c->xAxis()->setLabelAlignment(1);

    // Set the y axis stem and labels to transparent (that is, hide the labels)
    c->yAxis()->setColors(Chart::Transparent, Chart::Transparent);

    // Add a line layer for the lines with fast line mode enabled
    LineLayer *layer = c->addLineLayer();
    layer->setFastLineMode();

    // Now we add the 3 data series to a line layer, using the color red (0xff3333), green
    // (0x008800) and blue (0x3333cc)
    layer->setXData(DoubleArray(m_timeStamps, m_currentIndex));
    layer->addDataSet(DoubleArray(m_dataSeriesA, m_currentIndex), 0xff3333);
    layer->addDataSet(DoubleArray(m_dataSeriesB, m_currentIndex), 0x008800);

    // The x axis scales should reflect the full range of the view port
    c->xAxis()->setDateScale(vpc->getViewer()->getValueAtViewPort("x", 0),
        vpc->getViewer()->getValueAtViewPort("x", 1));
    c->xAxis()->setLabelFormat("{value|nn:ss}");

    // For the automatic x-axis labels, set the minimum spacing to 75 pixels.
    c->xAxis()->setTickDensity(75);

    // For the auto-scaled y-axis, as we hide the labels, we can disable axis rounding. This can
    // make the axis scale fit the data tighter.
    c->yAxis()->setRounding(false, false);

    // Output the chart
    delete vpc->getChart();
    vpc->setChart(c);
}




//
// Handles realtime data from RandomWalk. The RandomWalk will call this method from its own thread.
// This is a static method.
//
void MainWindow::OnData(void *self, double elapsedTime, double series0, double series1)
{
    /*
    // Copy the data into a structure and store it in the queue.
    DataPacket packet;
    packet.elapsedTime = elapsedTime;
    packet.series0 = series0;
    packet.series1 = series1;

    //buffer.put(packet);
    ((MainWindow *)self)->buffer.put(packet);*/
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
    //m_previewChart->setGeometry(15, 25, 640, 500);
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

     g_ExperimentParam.waveform = 0;
     g_ExperimentParam.amplitude = 0.1;
     g_ExperimentParam.frequency = 5.0;
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

    g_AccPeakValue = 0.0755;
    g_PosPeakValue = 0.1204;

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
        g_PosPeakValue = g_AccPeakValue + 0.1274;
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

//******************************************保存按钮按下则将当前绘图曲线保存
void MainWindow::onSave(bool)
{
    qDebug()<<"pressed onSave";
    QString fileName = QFileDialog::getSaveFileName(this, "Save", "chartdirector_demo",
        "PNG (*.png);;JPG (*.jpg);;GIF (*.gif);;BMP (*.bmp);;SVG (*.svg);;PDF (*.pdf)");

    if (!fileName.isEmpty())
    {
        // Save the chart
        BaseChart *c = m_ChartViewer->getChart();
        if (0 != c)
            c->makeChart(fileName.toUtf8().constData());
    }
}

/**
 * @brief MainWindow::on_Start_btn_clicked
 *开始按钮按下
 */
void MainWindow::on_Start_btn_clicked()
{

    QString msg("start");
    m_tcpmsgserver->SltMsgToClient(ProtocolSet::COMMAND,msg);
    //设置状态为开始运行
    //GlobalData::g_IsRunning = true;
    g_IsRunning = true;
    HintMsg_LineEdit->setPlaceholderText("试验正在进行.....");


}
/**
 * @brief MainWindow::on_Stop_btn_clicked
 */
void MainWindow::on_Stop_btn_clicked()
{
    QString msg("stop");
    m_tcpmsgserver->SltMsgToClient(ProtocolSet::COMMAND,msg);

    //GlobalData::g_IsRunning = false;
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
    connect(new_exp,new_experiment::ExperimentParamChangeSingal,this,ExperimentParamChangeSlot);
    new_exp->show();
}


void MainWindow::tcpsever_connect(const QString &ip)
{
    QStatusBar *statubar = this->statusBar();
    statubar->showMessage("新的控制器连接 IP:"+ip,3000);
    TcpServerAbstract_count++;
    QString sta = QString("当前控制器连接个数: %1").arg(TcpServerAbstract_count);
    tcpstatus->setText(sta);

    qDebug()<<"回调tcpsever_connect";

}

void MainWindow::tcpsever_disconnect(const QString &ip)
{
    QStatusBar *statubar = this->statusBar();
    //若出现错误
    TcpServerAbstract_count--;
    statubar->showMessage("控制器断开连接",3000);
    QString sta;
    if(TcpServerAbstract_count==0)
        sta.sprintf("未连接到控制器");
    else
        sta.sprintf("当前控制器连接个数: %d",TcpServerAbstract_count);
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
    QFile *report = new QFile("report.txt");
    if(!report->open(QIODevice::WriteOnly|QIODevice::Text))
    {
        qDebug()<<"Open file failed";
    }

    QTextStream out(report);
    out.setCodec("UTF-8");
    QString ExperimentIDStr = "试验编号";
    QString ExperimentID = QString::number(g_ExperimentID);

    QString WaveFormStr = "波形";
    QString WaveForm = "正弦波";

    QString ControlStrategyStr = "控制方法";
    QString ControlStrategy = "PID";

    QString ControlParamStr = "控制参数";
    QString P = "P\t0.25";
    QString I = "I\t0.25";
    QString D = "D\t0.25";

    QString ExperimentParamStr = "实验参数";
    QString Amplifier = QString("幅值\t%1%2").arg("25.0").arg("mm");
    QString Frequncy = QString("频率\t%1%2").arg("1.0").arg("Hz");
    QString Phase = QString("相位\t%1%2").arg("0.0").arg("");


    out <<ExperimentIDStr << endl;
    out <<ExperimentID << endl;
    out <<endl<<endl;


    out << WaveFormStr <<endl;
    out << WaveForm <<endl;
    out <<endl<<endl;

    out << ControlStrategyStr <<endl;
    out << ControlStrategy << endl;
    out <<endl<<endl;

    out << ControlParamStr << endl;
    out << P << endl;
    out << I << endl;
    out << D << endl;
    out <<endl<<endl;

    out << ExperimentParamStr << endl;
    out << Amplifier << endl;
    out << Frequncy << endl;
    out << Phase << endl;
    out <<endl<<endl;

    report->close();

    int ret=QMessageBox::information(this,"提示信息","成功生成报告文件",
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

void MainWindow::on_PreviewWave_PushButton_clicked()
{
    QString title;
    ui->DynamicPlotTabWidget->setCurrentIndex(1);

    QStringList waveModeString;
    waveModeString<<"Sine Wave"<<"Sine Sweep Wave"<<"Random Wave"<<"Triangle Wave";
    title = waveModeString[g_ExperimentParam.waveform];
    WavePreview(title);
}
