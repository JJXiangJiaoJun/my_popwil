#include "process_package.h"


#define DATA_SIZE sizeof(ChartDataType)
#define PLOT_INTERVAL 10   //每隔多少个点绘制一个点

void ProcessPackage::ParseDebugPosDataMsg(QDataStream &raw_data, FrameLengthType msg_len)
{
    ChartDataType data;
    msg_len /= DATA_SIZE;
    for(int i=0;i<msg_len;i++)
    {
        //将数据一个个读入缓冲区中
        raw_data >> data;
        qDebug() << data;
        //GlobalData::g_PosData.PushBack(data);
        g_PosData.PushBackToCurDataArray(data);
    }

}


void ProcessPackage::ParseDebugVelDataMsg(QDataStream &raw_data, FrameLengthType msg_len)
{
    ChartDataType data;
    msg_len /= DATA_SIZE;
    //qDebug()<<"收到数据为" <<msg_len;
    for(int i=0;i<msg_len;i++)
    {
        //将数据一个个读入缓冲区中
        raw_data >> data;
        //GlobalData::g_AccData.PushBack(data);
        g_VelData.PushBackToCurDataArray(data);
    }
}

void ProcessPackage::ParseDebugAccDataMsg(QDataStream &raw_data, FrameLengthType msg_len)
{
    ChartDataType data;
    msg_len /= DATA_SIZE;
    //qDebug()<<"收到数据为" <<msg_len;
    for(int i=0;i<msg_len;i++)
    {
        //将数据一个个读入缓冲区中
        raw_data >> data;
        //GlobalData::g_AccData.PushBack(data);
        g_AccData.PushBackToCurDataArray(data);
    }
}

void ProcessPackage::ParseCurPosDataMsg(QDataStream &raw_data, FrameLengthType msg_len)
{
    //将数据填充至绘图中
    //每隔10个点保存一个数据
    ChartDataType data;
    msg_len /= DATA_SIZE;
    for(int i=0;i<msg_len;i++)
    {
        //将数据一个个读入缓冲区中
        raw_data >> data;
        qDebug() << data;
        //GlobalData::g_PosData.PushBack(data);
        //每隔10个点保存一个点
        if(i%PLOT_INTERVAL == 0)
            g_PosData.PushBackToCurDataArray(data);
    }


}

void ProcessPackage::ParseCurVelDataMsg(QDataStream &raw_data, FrameLengthType msg_len)
{
    ChartDataType data;
    msg_len /= DATA_SIZE;
    //qDebug()<<"收到数据为" <<msg_len;
    for(int i=0;i<msg_len;i++)
    {
        //将数据一个个读入缓冲区中
        raw_data >> data;
        if(i%PLOT_INTERVAL == 0)
            g_VelData.PushBackToCurDataArray(data);
    }
}

void ProcessPackage::ParseCurAccDataMsg(QDataStream &raw_data, FrameLengthType msg_len)
{
    ChartDataType data;
    msg_len /= DATA_SIZE;
    //qDebug()<<"收到数据为" <<msg_len;
    for(int i=0;i<msg_len;i++)
    {
        //将数据一个个读入缓冲区中
        raw_data >> data;
        if(i%PLOT_INTERVAL == 0)
            g_AccData.PushBackToCurDataArray(data);
    }
}

/**
 * @brief ProcessPackage::ParseRunningPosDataMsg
 * @param raw_data
 * @param msg_len
 * 数据表示 [ref,cur,ref,cur,ref,cur,ref,cur,......]
 */
void ProcessPackage::ParseRunningPosDataMsg(QDataStream &raw_data, FrameLengthType msg_len)
{
    ChartDataType refPos;
    ChartDataType curPos;
    msg_len /=DATA_SIZE;
    FrameLengthType dataCnt = msg_len / 2;
    //需要保存到Running全局数组中
    for(int i=0;i<dataCnt;i++)
    {
        //一次读取一组数据
        raw_data >> refPos >> curPos;
        //加入到全局数组中进行保存
        g_PosRunningData.push(refPos,curPos);
        //加入到绘图数组
        if(i%PLOT_INTERVAL == 0)
        {
            g_PosData.PushBackToRefDataArray(refPos);
            g_PosData.PushBackToCurDataArray(curPos);
        }
    }
}

void ProcessPackage::ParseRunningVelDataMsg(QDataStream &raw_data, FrameLengthType msg_len)
{
    ChartDataType refVel;
    ChartDataType curVel;
    msg_len /=DATA_SIZE;
    FrameLengthType dataCnt = msg_len / 2;
    //需要保存到Running全局数组中
    for(int i=0;i<dataCnt;i++)
    {
        //一次读取一组数据
        raw_data >> refVel >> curVel;
        //加入到全局数组中进行保存
        g_VelRunningData.push(refVel,curVel);
        //加入到绘图数组
        if(i%PLOT_INTERVAL == 0)
        {
            g_VelData.PushBackToRefDataArray(refVel);
            g_VelData.PushBackToCurDataArray(curVel);
        }
    }
}

void ProcessPackage::ParseRunningAccDataMsg(QDataStream &raw_data, FrameLengthType msg_len)
{
    ChartDataType refAcc;
    ChartDataType curAcc;
    msg_len /=DATA_SIZE;
    FrameLengthType dataCnt = msg_len / 2;
    //需要保存到Running全局数组中
    for(int i=0;i<dataCnt;i++)
    {
        //一次读取一组数据
        raw_data >> refAcc >> curAcc;
        //加入到全局数组中进行保存
        g_AccRunningData.push(refAcc,curAcc);
        //加入到绘图数组
        if(i%PLOT_INTERVAL == 0)
        {
            g_AccData.PushBackToRefDataArray(refAcc);
            g_AccData.PushBackToCurDataArray(curAcc);
        }
    }
}


void ProcessPackage::ParseCommandMsg(QDataStream &raw_data, FrameLengthType msg_len)
{

}

/**
 * @brief ProcessPackage::ParseEchoMsg
 * @param raw_data
 * @param msg_len
 */
void ProcessPackage::ParseEchoMsg(QDataStream &raw_data, FrameLengthType msg_len)
{
    if(msg_len!=sizeof(qint16))
    {
        qDebug()<<"回复数据的类型不对 qint16";
    }
    qint16 echoMsg;
    raw_data >> echoMsg;
    qDebug()<<"接收到回复消息:"<<g_EchoStringList[echoMsg];
    switch (echoMsg) {
    case EchoMessageEnum::ReceiveRefData:

        break;
    case EchoMessageEnum::ExperimentStart:
        //清除原始数据进行重新绘图
        g_PosData.Clear();
        g_VelData.Clear();
        g_AccData.Clear();

        g_PosRunningData.Clear();
        g_VelRunningData.Clear();
        g_AccRunningData.Clear();

        g_IsRunning = true;
        g_PosPeakValue = 0.0;
        g_AccPeakValue = 0.0;

        g_PosRunningData = 0.0;
        g_AccRunningData = 0.0;
        g_VelRunningData = 0.0;
        break;
    case EchoMessageEnum::ExperimentStop:
        g_IsRunning = false;
    default:
        break;
    }
}
