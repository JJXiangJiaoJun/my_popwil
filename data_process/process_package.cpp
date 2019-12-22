#include "process_package.h"


#define DATA_SIZE sizeof(ChartDataType)



void ProcessPackage::ParseCurPosDataMsg(QDataStream &raw_data, FrameLengthType msg_len)
{
    //将数据填充至绘图中
//    QDataStream buffer(&msg);
//    buffer.setDevice(QIODevice::ReadOnly);
//    buffer.setByteOrder(QDataStream::BigEndian);

    ChartDataType data;
    msg_len /= DATA_SIZE;
    //qDebug()<<"收到数据为" <<msg_len;
    for(int i=1;i<=msg_len;i++)
    {
        //将数据一个个读入缓冲区中
        raw_data >> data;
        //GlobalData::g_PosData.PushBack(data);
        g_PosData.PushBackToCurDataArray(data);
    }


}

void ProcessPackage::ParseCurVelDataMsg(QDataStream &raw_data, quint16 msg_len)
{
    ChartDataType data;
    msg_len /= DATA_SIZE;
    //qDebug()<<"收到数据为" <<msg_len;
    for(int i=1;i<=msg_len;i++)
    {
        //将数据一个个读入缓冲区中
        raw_data >> data;
        //GlobalData::g_AccData.PushBack(data);
        g_VelData.PushBackToCurDataArray(data);
    }
}

void ProcessPackage::ParseCurAccDataMsg(QDataStream &raw_data, quint16 msg_len)
{
    ChartDataType data;
    msg_len /= DATA_SIZE;
    //qDebug()<<"收到数据为" <<msg_len;
    for(int i=1;i<=msg_len;i++)
    {
        //将数据一个个读入缓冲区中
        raw_data >> data;
        //GlobalData::g_AccData.PushBack(data);
        g_AccData.PushBackToCurDataArray(data);
    }
}


void ProcessPackage::ParseCommandMsg(QDataStream &raw_data, quint16 msg_len)
{

}

