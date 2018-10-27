#include "mybasewindow.h"
#include "QScreen"
#include <QDebug>

/**
 * @brief MyBaseWindow::MyBaseWindow  初始化标题栏并且将其添加到垂直布局中
 * @param parent
 */
MyBaseWindow::MyBaseWindow(QWidget *parent) : QWidget(parent)
{
    //隐藏标题栏
    this->setWindowFlags(Qt::FramelessWindowHint);

    initTitleBar();
    //添加到垂直布局中
    m_verticalLayout = new QVBoxLayout();
    m_verticalLayout->addWidget(m_titleBar);
    m_verticalLayout->addStretch();
    m_verticalLayout->setSpacing(0);
    m_verticalLayout->setContentsMargins(0, 0, 0, 0);
}


/**
 * @brief MyBaseWindow::~MyBaseWindow
 */
MyBaseWindow::~MyBaseWindow()
{

}

/**
 * @brief MyBaseWindow::initTitleBar  默认的初始化函数
 */
void MyBaseWindow::initTitleBar()
{
    m_titleBar = new MyTitleBar(this);
    m_titleBar->move(0,0);
    //设置标题内容
    m_titleBar->setTitleContent(QString("popwil振动平台控制系统"));
    m_titleBar->setTitleWidth(this->width());
    m_titleBar->setButtonType(MIN_MAX_BUTTON);
    //设置标题图标
    m_titleBar->setTitleIcon(":/my_icon/Ressources/my_ico/popwil.png");
    m_titleBar->setButtonMinBackground(":/my_icon/Ressources/48/117.png");
    m_titleBar->setButtonMaxBackground(":/my_icon/Ressources/48/112.png");
    m_titleBar->setButtonRestoreBackground(":/my_icon/Ressources/48/48.png");
    m_titleBar->setButtonCloseBackground(":/my_icon/Ressources/48/118.png");

    connect(m_titleBar, SIGNAL(signalButtonMinClicked()), this, SLOT(onButtonMinClicked()));
    connect(m_titleBar, SIGNAL(signalButtonRestoredClicked()), this, SLOT(onButtonRestoredClicked()));
    connect(m_titleBar, SIGNAL(signalButtonMaxClicked()), this, SLOT(onButtonMaxClicked()));
    connect(m_titleBar, SIGNAL(signalButtonCloseClicked()), this, SLOT(onButtonCloseClicked()));
}

/**
 * @brief MyBaseWindow::onButtonCloseClicked
 */
void MyBaseWindow::onButtonCloseClicked()
{
    close();
}

/**
 * @brief MyBaseWindow::onButtonMaxClicked
 */
void MyBaseWindow::onButtonMaxClicked()
{
    m_titleBar->saveRestoreInfo(this->pos(),QSize(this->width(),this->height()));
    QScreen *screen=QGuiApplication::primaryScreen ();
    //QRect desktopRect = QGuiApplication::primaryScreen()->availableGeometry();
    QRect desktopRect = screen->availableGeometry();
    QRect FactRect = QRect(desktopRect.x() - 3, desktopRect.y() - 3, desktopRect.width() + 6, desktopRect.height() + 6);
    setGeometry(FactRect);
    //设置标题栏长度
    m_titleBar->setTitleWidth(this->width());
}
/**
 * @brief MyBaseWindow::onButtonMinClicked
 */
void MyBaseWindow::onButtonMinClicked()
{
     showMinimized();
}


/**
 * @brief MyBaseWindow::onButtonRestoreClicked
 */
void MyBaseWindow::onButtonRestoredClicked()
{
    QPoint windowPos;
    QSize windowSize;
    m_titleBar->getRestoreInfo(windowPos,windowSize);
    this->setGeometry(QRect(windowPos,windowSize));
}

/**
 * @brief MyBaseWindow::addWindows
 * @param m_window
 */
void MyBaseWindow::addWindows(QWidget *m_window)
{
    m_verticalLayout->addWidget(m_window);
    setLayout(m_verticalLayout);
}
