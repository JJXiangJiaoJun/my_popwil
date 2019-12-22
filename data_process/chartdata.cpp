#include "chartdata.h"
#include <QDebug>


/**
 * @brief ChartData::ChartData
 * 构造函数
 */
#define CHART_DEBUG

ChartData::ChartData()
{
    m_lastData = 0;
    curCount = 0;
    curDataArrayLen = 0;
    refDataArrayLen = 0;
}


//ChartDataType ChartData::GetCurData()
//{
//#ifndef CHART_DEBUG
//    //保证参考波形和实际波形点数相对应，每次都读取一个点
//    if(m_curDataList.isEmpty()||m_refDataList.isEmpty())
//    {
//        return 0;
//    }
//#else
//    if(m_curDataList.isEmpty())
//    {
//        return 0;
//    }
//#endif
//    m_lastData = m_curDataList.takeFirst();
//    return m_lastData;

//}

//ChartDataType ChartData::GetRefData()
//{
//#ifndef CHART_DEBUG
//    //保证参考波形和实际波形点数相对应，每次都读取一个点
//    if(m_curDataList.isEmpty()||m_refDataList.isEmpty())
//    {
//        return 0;
//    }
//#else
//    if(m_refDataList.isEmpty())
//    {
//        return 0;
//    }
//#endif
//    m_lastData = m_refDataList.takeFirst();
//    return m_lastData;

//}

bool ChartData::GetCurData(ChartDataType &curData)
{
    if(curCount >= curDataArrayLen)
    {
        curData = 0.0;
        return false;
    }
    curData = m_curDataArray[curCount++];
    return true;
}

bool ChartData::GetRunningData(ChartDataType &curData, ChartDataType &refData)
{
#ifndef CHART_DEBUG
    curData = 0.0;
    refData = 0.0;
    //保证参考波形和实际波形点数相对应，每次都读取一个点
    if(curCount>=curDataArrayLen&&curCount>=refDataArrayLen)
        return false;
    if(curCount < curDataArrayLen&& curCount<refDataArrayLen)
    {
        curData = m_curDataArray[curCount];
        refData = m_refDataArray[curCount++];
    }
    else if (curCount < curDataArrayLen)
    {
        curData = m_curDataArray[curCount++];
    }
    else
    {
        refData = m_refDataArray[curCount++];
    }

#else
    if(curCount>=curDataArrayLen||curCount>=refDataArrayLen)
    {
        curData = -1.0;
        refData = -1.0;
        return false;
    }
    curData = m_curDataArray[curCount];
    refData = m_refDataArray[curCount++];
#endif
    return true;
}



void ChartData::Empty()
{
    ClearDataCount();
    EmptyCurDataArray();
    EmptyRefDataArray();
}

void ChartData::ClearDataCount()
{
    curCount = 0;
}

void ChartData::EmptyCurDataArray()
{
    curDataArrayLen = 0;
}

void ChartData::EmptyRefDataArray()
{
    refDataArrayLen = 0;
}

void ChartData::PushBackToCurDataArray(const ChartDataType &value)
{
    if(curDataArrayLen >= MAX_DATA_SIZE)
        curDataArrayLen = (double)curDataArrayLen * 0.8;
    m_curDataArray[curDataArrayLen++] = value;
}

void ChartData::PushBackToRefDataArray(const ChartDataType &value)
{
    if(refDataArrayLen >= MAX_DATA_SIZE)
        refDataArrayLen = (double)refDataArrayLen * 0.8;
    m_refDataArray[refDataArrayLen++] = value;
}

//void ChartData::PushBackToCurDataList(const ChartDataType &value)
//{
//    if(m_curDataList.size()>MAX_DATA_SIZE)
//    {
//        qDebug()<<"数据已满，不能插入更多的数据";
//        return;
//    }
//    m_curDataList.push_back(value);
//}

//void ChartData::PushBackToRefDataList(const ChartDataType &value)
//{
//    if(m_refDataList.size()>MAX_DATA_SIZE)
//    {
//        qDebug()<<"数据已满，不能插入更多的数据";
//        return;
//    }
//    m_refDataList.push_back(value);
//}






