#ifndef MYBASEWINDOW_H
#define MYBASEWINDOW_H

#include <QObject>
#include <QWidget>
#include "mytitlebar.h"
#include <QLayout>
#include <QVBoxLayout>
#include <QGuiApplication>
/**
 * @brief The MyBaseWindow class
 * 基本窗口， MyTitleBar+下面的窗口
 *
 */
class MyBaseWindow : public QWidget
{
    Q_OBJECT
public:
     MyBaseWindow(QWidget *parent = nullptr);
     ~MyBaseWindow();
public:
     MyTitleBar *m_titleBar;
     QVBoxLayout *m_verticalLayout;
public:
     void addWindows(QWidget *m_window);
signals:

private slots:
     //标题栏按钮
     void onButtonMinClicked();
     void onButtonRestoredClicked();
     void onButtonMaxClicked();
     void onButtonCloseClicked();
private:
      void initTitleBar();

};

#endif // MYBASEWINDOW_H
