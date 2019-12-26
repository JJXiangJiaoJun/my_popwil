#include "myhelper.h"

/**
 * @brief myHelper::ReadFromSystemInfoIni
 * @param fineName
 */
bool myHelper::ReadFromSystemInfoIni(QString fileName,SystemInfoStruct *sysInfo)
{
    if(fileName==NULL||fileName=="")
    {
          QMessageBox::information(NULL,"警告","文件名为空,请重新输入",QMessageBox::Ok|QMessageBox::Cancel);
          qDebug()<<"Ini文件名为空";
          return false;
    }
    QSettings IniSettingReader(fileName,QSettings::IniFormat);
    IniSettingReader.beginGroup(QString("SystemInfo"));
    sysInfo->controlInterval = IniSettingReader.value("ControlInterval").toDouble();
    sysInfo->drawInterval = IniSettingReader.value("drawInterval").toDouble();
    sysInfo->maxAbsoluteAcc = IniSettingReader.value("maxAbsoluteAcc").toDouble();
    sysInfo->maxAbsolutePosition = IniSettingReader.value("maxAbsolutePosition").toDouble();
    sysInfo->maxAbsoluteVel = IniSettingReader.value("maxAbsoluteVel").toDouble();
    sysInfo->samplePeriod = IniSettingReader.value("samplePeriod").toDouble();
}

/**
 * @brief myHelper::WriteToSystemInfoIni
 * @param fileName
 * @param sysInfo
 */
bool myHelper::WriteToSystemInfoIni(QString fileName, SystemInfoStruct sysInfo)
{
    if(fileName==NULL||fileName=="")
    {
          QMessageBox::information(NULL,"警告","文件名为空,请重新输入",QMessageBox::Ok|QMessageBox::Cancel);
          qDebug()<<"Ini文件名为空";
          return false;
    }
    QSettings IniSettingWriter(fileName,QSettings::IniFormat);
    IniSettingWriter.beginGroup(QString("SystemInfo"));
    IniSettingWriter.setValue("ControlInterval",sysInfo.controlInterval);
    IniSettingWriter.setValue("drawInterval",sysInfo.drawInterval);
    IniSettingWriter.setValue("maxOutU",sysInfo.maxOutU);
    IniSettingWriter.setValue("maxAbsoluteAcc",sysInfo.maxAbsoluteAcc);
    IniSettingWriter.setValue("maxAbsoluteVel",sysInfo.maxAbsoluteVel);
    IniSettingWriter.setValue("maxAbsolutePosition",sysInfo.maxAbsolutePosition);
    IniSettingWriter.setValue("samplePeriod",sysInfo.samplePeriod);
    IniSettingWriter.endGroup();

    return true;
}


bool myHelper::ReadFromPIDParamIni(QString fileName, PIDParamStruct *staticPID,
                                   PIDParamStruct *posPID, PIDParamStruct *accPID)
{
    if(fileName==NULL||fileName=="")
    {
          QMessageBox::information(NULL,"警告","文件名为空,请重新输入",QMessageBox::Ok|QMessageBox::Cancel);
          qDebug()<<"Ini文件名为空";
          return false;
    }
    QSettings PIDSettingReader(fileName,QSettings::IniFormat);
    PIDSettingReader.beginGroup(QString("PIDParam"));

    staticPID->P = PIDSettingReader.value(QString("StaticPID_P")).toDouble();
    staticPID->I = PIDSettingReader.value(QString("StaticPID_I")).toDouble();
    staticPID->D = PIDSettingReader.value(QString("StaticPID_D")).toDouble();

    posPID->P = PIDSettingReader.value(QString("PosPID_P")).toDouble();
    posPID->I = PIDSettingReader.value(QString("PosPID_I")).toDouble();
    posPID->D = PIDSettingReader.value(QString("PosPID_D")).toDouble();

    accPID->P = PIDSettingReader.value(QString("AccPID_P")).toDouble();
    accPID->I = PIDSettingReader.value(QString("PosPID_I")).toDouble();
    accPID->D = PIDSettingReader.value(QString("PosPID_D")).toDouble();

    PIDSettingReader.endGroup();
    return true;
}


