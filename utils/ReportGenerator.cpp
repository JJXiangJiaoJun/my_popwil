#include "ReportGenerator.h"

ReportGenerator::ReportGenerator()
{

}


bool ReportGenerator::GenerateCSVReport(QString fileName, QString sep)
{
    QFile csvFile(fileName);
    if(!csvFile.open(QIODevice::WriteOnly|QIODevice::Text))
        return false;
    //把试验信息写入文件

    QTextStream fileWriter(&csvFile);
    fileWriter.setCodec(QTextCodec::codecForName("UTF-8"));
    //表头
    fileWriter << "Cur_Pos" << sep<<"Ref_Pos" <<sep<<"Cur_Vel"<<sep \
               <<"Ref_Vel"<<sep<<"Cur_Acc"<<sep<<"Ref_Acc"<<endl;

//    fileWriter << 1.251<<sep<<1.253<<sep<<25.142<<sep \
//               <<1231.14<<sep<<1.2554<<sep<<12313<<endl;
    //fileWriter.flush();
    qint32 dataCnt = g_PosRunningData.dataCnt;
    for(int i=0;i<dataCnt;i++)
    {
        fileWriter<<g_PosRunningData.CurBuffer[i] << sep \
                  <<g_PosRunningData.RefBuffer[i] << sep \
                  <<g_VelRunningData.CurBuffer[i] << sep \
                  <<g_VelRunningData.RefBuffer[i] << sep \
                  <<g_AccRunningData.CurBuffer[i] << sep \
                  <<g_AccRunningData.RefBuffer[i] << endl;
    }

    csvFile.flush();
    csvFile.close();
    return true;
}
