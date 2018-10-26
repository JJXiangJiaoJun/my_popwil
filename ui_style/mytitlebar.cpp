#include "mytitlebar.h"
#include <QHBoxLayout>
#include <QPainter>
#include <QFile>
#include <QMouseEvent>


#define BUTTON_HEIGHT  35
#define BUTTON_WIDTH   35
#define TITLE_HEIGHT   35


/**
 * @brief MyTitleBar::MyTitleBar
 * @param parent  作为parent的标题栏
 */
MyTitleBar::MyTitleBar(QWidget *parent) :
    QWidget(parent),
    m_colorR(139),
    m_colorG(187),
    m_colorB(209),
    m_isPressed(false),
    m_buttonType(MIN_MAX_BUTTON),
    m_windowBorderWidth(0),
    m_isTransparent(false)
{
        initControl();
        initConnections();
        //jia
}
/**
 * @brief MyTitleBar::~MyTitleBar
 */
MyTitleBar::~MyTitleBar()
{

}
/**
 * @brief MyTitleBar::initControl
 * 初始化控件
 */
void MyTitleBar::initControl()
{
    m_pIcon =new QLabel;
    m_pTitleContent = new QLabel;

    m_pButtonClose =new QPushButton;
    m_pButtonMax = new QPushButton;
    m_pButtonMin = new QPushButton;
    m_pButtonRestore = new QPushButton;

    m_pIcon->setScaledContents(true);
    m_pButtonClose->setFixedSize(BUTTON_WIDTH,BUTTON_HEIGHT);
    m_pButtonMax->setFixedSize(BUTTON_WIDTH,BUTTON_HEIGHT);
    m_pButtonMin->setFixedSize(BUTTON_WIDTH,BUTTON_HEIGHT);
    m_pButtonRestore->setFixedSize(BUTTON_WIDTH,BUTTON_HEIGHT);

    this->setObjectName(tr("TitleBar"));
    m_pIcon->setObjectName(tr("TitleIcon"));
    m_pTitleContent->setObjectName(tr("TitleContent"));
    m_pButtonMin->setObjectName(tr("ButtonMin"));
    m_pButtonRestore->setObjectName(tr("ButtonRestore"));
    m_pButtonMax->setObjectName(tr("ButtonMax"));
    m_pButtonClose->setObjectName(tr("ButtonClose"));

    m_pButtonMin->setToolTip(QStringLiteral("最小化"));
    m_pButtonRestore->setToolTip(QStringLiteral("还原"));
    m_pButtonMax->setToolTip(QStringLiteral("最大化"));
    m_pButtonClose->setToolTip(QStringLiteral("关闭"));

    QHBoxLayout *mylayout = new QHBoxLayout(this);
    mylayout->addWidget(m_pIcon);
    mylayout->addWidget(m_pTitleContent);

    mylayout->addWidget(m_pButtonMin);
    mylayout->addWidget(m_pButtonRestore);
    mylayout->addWidget(m_pButtonMax);
    mylayout->addWidget(m_pButtonClose);

    mylayout->setContentsMargins(5,0,0,0);
    mylayout->setSpacing(0);

    m_pTitleContent->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Fixed);
    this->setFixedHeight(TITLE_HEIGHT);
    this->setWindowFlags(Qt::FramelessWindowHint);

}
/**
 * @brief MyTitleBar::initConnections
 * 信号槽的绑定
 */
void MyTitleBar::initConnections()
{
    connect(m_pButtonMin,SIGNAL(clicked()),this,SLOT(onButtonMinClicked()));
    connect(m_pButtonRestore,SIGNAL(clicked()),this,SLOT(onButtonRestoredClicked()));
    connect(m_pButtonMax,SIGNAL(clicked()),this,SLOT(onButtionMaxClicked()));
    connect(m_pButtonClose,SIGNAL(clicked()),this,SLOT(onButtonCloseClicked()));
}


/**
 * @brief MyTitleBar::setBackgroundColor 设置标题栏背景色，在paintEvent实践中绘制标题栏背景色
 * @param r
 * @param g
 * @param b
 * @param isTransparent
 */
void MyTitleBar::setBackgroundColor(int r, int g, int b, bool isTransparent)
{
    m_colorR=r;
    m_colorG=g;
    m_colorB=b;

    m_isTransparent=isTransparent;
    update();
}
/**
 * @brief MyTitleBar::setTitleIcon  设置标题栏图标
 * @param filePath
 * @param IconSize
 */
void MyTitleBar::setTitleIcon(QString filePath, QSize IconSize)
{
    QPixmap titleIcon(filePath);
    m_pIcon->setPixmap(titleIcon.scaled(IconSize));
}

void MyTitleBar::setTileIconSize(QSize IconSize)
{
    m_pIcon->setFixedSize(QSize(IconSize));
}

/**
 * @brief MyTitleBar::setTitleContent 设置标题栏内容
 * @param titleContent
 * @param titleFontSize
 */
void MyTitleBar::setTitleContent(QString titleContent, int titleFontSize)
{
    QFont font = m_pTitleContent->font();
    font.setPointSize(titleFontSize);
    m_pTitleContent->setFont(font);

    m_pTitleContent->setText(titleContent);
    m_titleContent = titleContent;
}

/**
 * @brief MyTitleBar::setTitleWidth  设置标题栏长度
 * @param width
 */
void MyTitleBar::setTitleWidth(int width)
{
    this->setFixedWidth(width);
}
/**
 * @brief MyTitleBar::setButtonType  设置标题栏类型
 * 提供四个按钮分别为最小化、还原、最大化、关闭按钮
 *
 * @param buttonType
 */
void MyTitleBar::setButtonType(ButtonType buttonType)
{
    m_buttonType =buttonType;

    switch(buttonType)
    {
        case MIN_BUTTON:
            m_pButtonRestore->setVisible(false);
            m_pButtonMax->setVisible(false);
            break;
        case MIN_MAX_BUTTON:
            m_pButtonRestore->setVisible(false);
            break;
        case ONLY_CLOSE_BUTTON:
            m_pButtonMin->setVisible(false);
            m_pButtonMax->setVisible(false);
            m_pButtonRestore->setVisible(false);
            break;
        default:
            break;
    }
}

void MyTitleBar::setButtonMinBackground(QString filepath)
{
    m_pButtonMin->setIcon(QIcon(filepath));
}

void MyTitleBar::setButtonCloseBackground(QString filepath)
{
    m_pButtonClose->setIcon(QIcon(filepath));
}

void MyTitleBar::setButtonMaxBackground(QString filepath)
{
    m_pButtonMax->setIcon(QIcon(filepath));
}

void MyTitleBar::setButtonRestoreBackground(QString filepath)
{
    m_pButtonRestore->setIcon(QIcon(filepath));
}

/**
 * @brief MyTitleBar::setTitleRoll   自定义标题栏滚动
 */
void MyTitleBar::setTitleRoll()
{
    connect(&m_titleRollTimer,SIGNAL(timeout()),this,SLOT(onRollTitle()));
    m_titleRollTimer.start(200);
}

/**
 * @brief MyTitleBar::setWindowBorderWidth 设置标题栏边框宽度
 * @param borderWidth
 */
void MyTitleBar::setWindowBorderWidth(int borderWidth)
{
    m_windowBorderWidth = borderWidth;
}

/**
 * @brief MyTitleBar::saveRestoreInfo  保存窗口最大化前窗口位置
 * @param point
 * @param size
 */
void MyTitleBar::saveRestoreInfo(const QPoint point, const QSize size)
{
    m_restorePos = point;
    m_restoreSize=size;
}

void MyTitleBar::getRestoreInfo(QPoint &point, QSize &size)
{
     point = m_restorePos;
     size = m_restoreSize;
}

/**
 * @brief MyTitleBar::paintEvent  重绘时间绘制标题栏背景色
 * @param event
 */
void MyTitleBar::paintEvent(QPaintEvent *event)
{
    if(!m_isTransparent)
    {
        QPainter painter(this);
        QPainterPath pathBack;
        pathBack.setFillRule(Qt::WindingFill);
        pathBack.addRoundedRect(QRect(0,0,this->width(),this->height()),3,3);
        painter.setRenderHint(QPainter::Antialiasing,true);
        painter.fillPath(pathBack,QBrush(QColor(m_colorR,m_colorG,m_colorB)));

    }

    // 当窗口最大化或者还原后，窗口长度变了，标题栏的长度应当一起改变;
    // 这里减去m_windowBorderWidth ，是因为窗口可能设置了不同宽度的边框;
    // 如果窗口有边框则需要设置m_windowBorderWidth的值，否则m_windowBorderWidth默认为0;
    if (this->width() != (this->parentWidget()->width() - m_windowBorderWidth))
    {
        this->setFixedWidth(this->parentWidget()->width() - m_windowBorderWidth);
    }
}

/**
 * @brief MyTitleBar::mouseDoubleClickEvent 实现双击标题栏进行最大化最小化操作
 * @param event
 */
void MyTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    //只有存在最大化，还原按钮时双击才有效
    if(m_buttonType = MIN_MAX_BUTTON)
    {
        if(m_pButtonMax->isVisible())
        {
            onButtionMaxClicked();
        }
        else
        {
            onButtonRestoredClicked();
        }
    }

    return QWidget::mouseDoubleClickEvent(event);
}



/// 以下通过mousePressEvent、mouseMoveEvent、mouseReleaseEvent三个事件实现了鼠标拖动标题栏移动窗口的效果;

/**
 * @brief MyTitleBar::mousePressEvent
 * @param event
 */
void MyTitleBar::mousePressEvent(QMouseEvent *event)
{
    if(m_buttonType ==MIN_MAX_BUTTON)
    {
        //窗口最大化时禁止拖动窗口
        if(m_pButtonMax->isVisible())
        {
            m_isPressed = true;
            m_startMovePos = event->globalPos();
        }
    }
    else
    {
        m_isPressed =true;
        m_startMovePos = event->globalPos();
    }
    return QWidget::mousePressEvent(event);
}

/**
 * @brief MyTitleBar::mouseMoveEvent
 * @param event
 */
void MyTitleBar::mouseMoveEvent(QMouseEvent *event)
{
    if(m_isPressed)
    {
        QPoint movePoint = event->globalPos()-m_startMovePos;
        QPoint widgetPos = this->parentWidget()->pos();
        m_startMovePos = event->globalPos();
        this->parentWidget()->move(widgetPos.x()+movePoint.x(),widgetPos.y()+movePoint.y());
    }
    return QWidget::mouseMoveEvent(event);
}
/**
 * @brief MyTitleBar::mouseReleaseEvent
 * @param event
 */
void MyTitleBar::mouseReleaseEvent(QMouseEvent *event)
{
    m_isPressed = false;
    return QWidget::mouseReleaseEvent(event);
}

/**
 * @brief MyTitleBar::setStyleSheet  加载样式文件
 * @param styleSheet
 */
void MyTitleBar::loadStyleSheet(const QString &stylesheetName)
{

}

///以下是按钮操作相应的槽

void MyTitleBar::onButtonMinClicked()
{
    emit signalButtonMinClicked();
}


void MyTitleBar::onButtonRestoredClicked()
{
    m_pButtonRestore->setVisible(false);
    m_pButtonMax->setVisible(true);
    emit signalButtonRestoredClicked();
}

void MyTitleBar::onButtionMaxClicked()
{
    m_pButtonMax->setVisible(false);
    m_pButtonRestore->setVisible(true);
    emit signalButtonMaxClicked();
}

void MyTitleBar::onButtonCloseClicked()
{
    emit signalButtonCloseClicked();
}



/**
 * @brief MyTitleBar::onRollTitle  该方法主要是让标题栏中的标题显示为滚动的效果;
 */
void MyTitleBar::onRollTitle()
{
    static int nPos = 0;
    QString titleContent = m_titleContent;
    // 当截取的位置比字符串长时，从头开始;
    if (nPos > titleContent.length())
        nPos = 0;

    m_pTitleContent->setText(titleContent.mid(nPos));
    nPos++;
}












