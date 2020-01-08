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
    accPID->I = PIDSettingReader.value(QString("AccPID_I")).toDouble();
    accPID->D = PIDSettingReader.value(QString("AccPID_D")).toDouble();

    PIDSettingReader.endGroup();
    return true;
}

bool myHelper::WriteToPIDParamIni(QString fileName, PIDParamStruct *staticPID,
                                  PIDParamStruct *posPID, PIDParamStruct *accPID)
{
    if(fileName==NULL||fileName=="")
    {
          QMessageBox::information(NULL,"警告","文件名为空,请重新输入",QMessageBox::Ok|QMessageBox::Cancel);
          qDebug()<<"Ini文件名为空";
          return false;
    }
    QSettings PIDSettingWriter(fileName,QSettings::IniFormat);
    PIDSettingWriter.beginGroup(QString("PIDParam"));
    PIDSettingWriter.setValue("staticPID_P",staticPID->P);
    PIDSettingWriter.setValue("staticPID_I",staticPID->I);
    PIDSettingWriter.setValue("staticPID_D",staticPID->D);

    PIDSettingWriter.setValue("PosPID_P",posPID->P);
    PIDSettingWriter.setValue("PosPID_I",posPID->I);
    PIDSettingWriter.setValue("PosPID_D",posPID->D);

    PIDSettingWriter.setValue("AccPID_P",accPID->P);
    PIDSettingWriter.setValue("AccPID_I",accPID->I);
    PIDSettingWriter.setValue("AccPID_D",accPID->D);

    PIDSettingWriter.endGroup();
    return true;
}


bool myHelper::ReadFromStaticVoltageIni(QString fileName, double *staticV)
{
    if(fileName==NULL||fileName=="")
    {
          QMessageBox::information(NULL,"警告","文件名为空,请重新输入",QMessageBox::Ok|QMessageBox::Cancel);
          qDebug()<<"Ini文件名为空";
          return false;
    }
    QSettings StaticVSettingReader(fileName,QSettings::IniFormat);
    StaticVSettingReader.beginGroup(QString("StaticVoltage"));
    *staticV = StaticVSettingReader.value("StaticV").toDouble();
    StaticVSettingReader.endGroup();
    return true;
}


bool myHelper::WriteToStaticVoltageIni(QString fileName, double *staticV)
{
    if(fileName==NULL||fileName=="")
    {
          QMessageBox::information(NULL,"警告","文件名为空,请重新输入",QMessageBox::Ok|QMessageBox::Cancel);
          qDebug()<<"Ini文件名为空";
          return false;
    }
    QSettings StaticVSettingWriter(fileName,QSettings::IniFormat);
    StaticVSettingWriter.beginGroup(QString("StaticVoltage"));
    StaticVSettingWriter.setValue("StaticV",*staticV);
    StaticVSettingWriter.endGroup();
    return true;
}

bool myHelper::ReadFromControlMethodIni(QString fileName, qint16 *controlMethod)
{
    if(fileName==NULL||fileName=="")
    {
          QMessageBox::information(NULL,"警告","文件名为空,请重新输入",QMessageBox::Ok|QMessageBox::Cancel);
          qDebug()<<"Ini文件名为空";
          return false;
    }
    QSettings ControlMethodSettingReader(fileName,QSettings::IniFormat);
    ControlMethodSettingReader.beginGroup(QString("ControlMethod"));
    *controlMethod = ControlMethodSettingReader.value("ctrlMethod").toInt();
    ControlMethodSettingReader.endGroup();
    return true;
}

bool myHelper::WriteToControlMethodIni(QString fileName, qint16 *controlMethod)
{
    if(fileName==NULL||fileName=="")
    {
          QMessageBox::information(NULL,"警告","文件名为空,请重新输入",QMessageBox::Ok|QMessageBox::Cancel);
          qDebug()<<"Ini文件名为空";
          return false;
    }
    QSettings ControlMethodSettingWriter(fileName,QSettings::IniFormat);
    ControlMethodSettingWriter.beginGroup(QString("ControlMethod"));
    ControlMethodSettingWriter.setValue("ctrlMethod",*controlMethod);
    ControlMethodSettingWriter.endGroup();
    return true;
}

bool myHelper::ReadFromControlVariableIni(QString fileName, qint16 *controlVariable)
{
    if(fileName==NULL||fileName=="")
    {
          QMessageBox::information(NULL,"警告","文件名为空,请重新输入",QMessageBox::Ok|QMessageBox::Cancel);
          qDebug()<<"Ini文件名为空";
          return false;
    }
    QSettings ControlVariableSettingReader(fileName,QSettings::IniFormat);
    ControlVariableSettingReader.beginGroup(QString("ControlVariable"));
    *controlVariable = ControlVariableSettingReader.value("ctrlVariable").toInt();
    ControlVariableSettingReader.endGroup();
    return true;
}


bool myHelper::WriteToControlVariableIni(QString fileName, qint16 *controlVariable)
{
    if(fileName==NULL||fileName=="")
    {
          QMessageBox::information(NULL,"警告","文件名为空,请重新输入",QMessageBox::Ok|QMessageBox::Cancel);
          qDebug()<<"Ini文件名为空";
          return false;
    }
    QSettings ControlVariableSettingWriter(fileName,QSettings::IniFormat);
    ControlVariableSettingWriter.beginGroup(QString("ControlVariable"));
    ControlVariableSettingWriter.setValue("ctrlVariable",*controlVariable);
    ControlVariableSettingWriter.endGroup();
    return true;
}
