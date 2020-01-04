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
    //如果当前数据或者参考数据不够,则等待不读取
    if(curCount>=curDataArrayLen||curCount>=refDataArrayLen)
        return false;
    else
    {
        curData = m_curDataArray[curCount];
        refData = m_refDataArray[curCount++];
    }
//    else if (curCount < curDataArrayLen)
//    {
//        curData = m_curDataArray[curCount++];
//    }
//    else
//    {
//        refData = m_refDataArray[curCount++];
//    }

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



void ChartData::Clear()
{
    ClearDataCount();
    ClearCurDataArray();
    ClearRefDataArray();
}

void ChartData::ClearDataCount()
{
    curCount = 0;
}

void ChartData::ClearCurDataArray()
{
    curDataArrayLen = 0;
}

void ChartData::ClearRefDataArray()
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








