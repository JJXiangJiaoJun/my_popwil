#include "chartdata.h"
#include <QDebug>


/**
 * @brief ChartData::ChartData
 * 构造函数
 */
ChartData::ChartData()
{
    m_lastData = 0;
}


double ChartData::TakeFirst()
{
    //如果链表中无内容那么返回默认值
    //qDebug()<<"最后的数据为 " << m_lastData;
    if(m_dataList.isEmpty())
    {
//        qDebug()<<"链表无数据为 " << m_lastData;
        return m_lastData;
    }
    //获取数据
    qDebug()<<"总数据长度" <<m_dataList.size();
    m_lastData = m_dataList.takeFirst();
    

    qDebug()<<"读取新数据 " << m_lastData;

    //读到头部数据后将已经读到的数据清除
    return m_lastData;
}


void ChartData::Empty()
{
    while(!m_dataList.isEmpty())
        m_dataList.pop_front();
}

void ChartData::PushBack(const ChartDataType &value)
{
    if(m_dataList.size()>MAX_DATA_SIZE)
    {
        qDebug()<<"数据已满，不能插入更多的数据";
        return;
    }

    m_dataList.push_back(value);
}




