#ifndef MYTITLEBAR_H
#define MYTITLEBAR_H

#include <QObject>
#include <QWidget>
#include <QString>
#include <QSize>
#include <QDebug>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QSize>
#include <QFrame>


/**
 * @brief The ButtonType enum
 */
enum ButtonType{
    MIN_BUTTON =0,          ///最小化按钮
    MIN_MAX_BUTTON,         ///最大最小化按钮
    ONLY_CLOSE_BUTTON       ///只有关闭按钮
};


/**
 * @brief The MyTitleBar class
 * 自定义标题栏
 */
class MyTitleBar : public QWidget
{
    Q_OBJECT
public:
    /// parent保证创建MyTtileBar对象时父指针必须赋值
    MyTitleBar(QWidget *parent);
     ~MyTitleBar();

    //设置标题栏的背景颜色
    void setBackgroundColor(int r,int g,int b,bool isTransparent = false);
    //设置标题栏图标
    void setTitleIcon(QString filePath,QSize IconSize = QSize(30,30));
    //设置标题内容
    void setTitleContent(QString titleContent,int titleFontSize=12);
    //设置标题栏长度
    void setTitleWidth(int width);
    //设置标题栏按钮类型
    void setButtonType(ButtonType buttonType);
    //设置标题栏滚动
    void setTitleRoll();
    //设置窗口边框宽度
    void setWindowBorderWidth(int borderWidth);

    //保存/获取  最大化前窗口的位置以及大小
    void saveRestoreInfo(const QPoint point,const QSize size);
    void getRestoreInfo(QPoint &point,QSize &size);

    void setTileIconSize(QSize IconSize = QSize(30,30));

    void setButtonMinBackground(QString filepath);
    void setButtonMaxBackground(QString filepath);
    void setButtonCloseBackground(QString filepath);
    void setButtonRestoreBackground(QString filepath);
private:

    void paintEvent(QPaintEvent *event);
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    //初始化控件
    void initControl();
    //信号槽的绑定
    void initConnections();
    //加载样式文件
    void loadStyleSheet(const QString &stylesheetName);



signals:
    void signalButtonMinClicked();
    void signalButtonRestoredClicked();
    void signalButtonMaxClicked();
    void signalButtonCloseClicked();
private slots:
    void onButtonMinClicked();
    void onButtonRestoredClicked();
    void onButtionMaxClicked();
    void onButtonCloseClicked();
    void onRollTitle();
private:
    QLabel *m_pIcon;                           //标题栏图表
    QLabel *m_pTitleContent;                   //标题栏内容
    QPushButton *m_pButtonMin;                 //最小化按钮
    QPushButton *m_pButtonRestore;             //最大化还原按钮
    QPushButton *m_pButtonMax;                 //最大化按钮
    QPushButton *m_pButtonClose;                //关闭按钮

    //标题栏背景颜色
    int m_colorR;
    int m_colorG;
    int m_colorB;

    //最大化、最小化变量
    QPoint m_restorePos;
    QSize m_restoreSize;

    //移动窗口的变量
    bool m_isPressed;
    QPoint m_startMovePos;

    //标题栏跑马灯效果时钟
    QTimer m_titleRollTimer;
    //标题栏内容
    QString m_titleContent;
    //按钮类型
    ButtonType m_buttonType;
    //窗口边框宽度
    int m_windowBorderWidth;
    //窗口是否透明
    bool m_isTransparent;

};

#endif // MYTITLEBAR_H
