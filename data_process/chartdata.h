#ifndef CHARTDATA_H
#define CHARTDATA_H

#include <QList>
#include <QtGlobal>
#include "global_setting.h"
//#include "myhelper.h"
#define DEFAULT_DATA 0.0
#define MAX_DATA_SIZE  1000000


class ChartData
{
public:
    ChartData();
private:
    QList<ChartDataType>  m_dataList;        //保存实际数据的list
    ChartDataType m_lastData;

public:
    ChartDataType TakeFirst();             //获取m_dataList中的一条数据,在定时器中断中调用
    void Empty();                   //清空m_dataList中的所有数据。
    void PushBack(const ChartDataType &value);   //向m_dataList中添加一条数据

};


#endif // CHARTDATA_H
