#ifndef CHARTDATA_H
#define CHARTDATA_H

#include <QList>
#include <QtGlobal>
#include "global_setting.h"
//#include "myhelper.h"
#define DEFAULT_DATA 0.0
#define MAX_DATA_SIZE  100000


class ChartData
{
public:
    ChartData();
private:
    //QList<ChartDataType>  m_curDataList;        //保存实际数据的list,实时值
    //QList<ChartDataType>  m_refDataList;        //参考值

    ChartDataType m_curDataArray[MAX_DATA_SIZE];
    ChartDataType m_refDataArray[MAX_DATA_SIZE];

    qint32 curDataArrayLen;
    qint32 refDataArrayLen;

    qint32 curCount;

    ChartDataType m_lastData;

public:
    //ChartDataType TakeFirst();             //获取m_dataList中的一条数据,在定时器中断中调用
    //ChartDataType GetCurData();              //获取m_dataList中的一条数据,在定时器中断中调用
    //ChartDataType GetRefData();              //获取m_dataList中的一条数据,在定时器中断中调用

    bool GetCurData(ChartDataType &curData);
    bool GetRunningData(ChartDataType &curData,ChartDataType &refData);

    void Empty();                   //清空ChartData中的所有数据。
    void EmptyCurDataArray();
    void EmptyRefDataArray();
    void ClearDataCount();

    //void PushBackToCurDataList(const ChartDataType &value);   //向m_curDataList中添加一条数据
    //void PushBackToRefDataList(const ChartDataType &value);   //向m_refDataList中添加一条数据
    void PushBackToCurDataArray(const ChartDataType &value);
    void PushBackToRefDataArray(const ChartDataType &value);

};


#endif // CHARTDATA_H
