#ifndef MYHELPER_H
#define MYHELPER_H


#include <stdlib.h>
#include <string>
#include <stdio.h>
#include <QObject>
#include <QString>
#include <QFile>
#include <QIODevice>
#include <QApplication>
#include <QStyleFactory>
#include <QTextStream>
#include <QTextCodec>
#include <QDebug>
#include <QSettings>
#include <QMessageBox>


#include "chartdata.h"
#include "global_setting.h"


//typedef double ChartDataType;

/**
 * @brief The myHelper class
 * 设置一些启动参数
 *
 */
class myHelper : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief setStyle 设置整个软件的样式表
     * @param style 样式表名称
     */
    static void setStyle(const QString &style)
    {
        //打开样式表路径
        QFile file(":/qss/Ressources/qss/"+style+".css");
        file.open(QIODevice::ReadOnly);
        qApp->setStyleSheet(file.readAll());
        file.close();
    }

    static bool ReadFromSystemInfoIni(QString fileName,SystemInfoStruct *sysInfo);
    static bool WriteToSystemInfoIni(QString fileName,SystemInfoStruct sysInfo);

    static bool ReadFromPIDParamIni(QString fileName,PIDParamStruct *staticPID,
                                      PIDParamStruct *posPID,PIDParamStruct *accPID);
signals:

public slots:
};






#endif // MYHELPER_H
