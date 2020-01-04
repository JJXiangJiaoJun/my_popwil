#include "control_param.h"
#include "ui_control_param.h"


control_param::control_param(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::control_param)
{
    ui->setupUi(this);

    P_StaticControl_LineEdit = ui->P_StaticControl_LineEdit;
    I_StaticControl_LineEdit = ui->I_StaticControl_LineEdit;
    D_StaticControl_LineEdit = ui->D_StaticControl_LineEdit;

    P_PosControl_LineEdit    = ui->P_PosControl_LineEdit;
    I_PosControl_LineEdit    = ui->I_PosControl_LineEdit;
    D_PosControl_LineEdit    = ui->D_PosControl_LineEdit;

    P_AccControl_LineEdit    = ui->P_AccControl_LineEdit;
    I_AccControl_LineEdit    = ui->I_AccControl_LineEdit;
    D_AccControl_LineEdit    = ui->D_AccControl_LineEdit;

    StaticVoltage_LineEdit = ui->StaticVoltage_LineEdit;

    PID_RadioButton = ui->PID_RadioButton;
    TVC_RadioButton = ui->TVC_RadioButton;
    ThreePID_RadioButton = ui->ThreePID_RadioButton;
    IterativeControl_RadioButton = ui->IterativeControl_RadioButton;
    PosVariable_RadioButton = ui->PosVariable_RadioButton;
    FVariable_RadioButton = ui->FVariable_RadioButton;

    //设置初始的数值
    QDoubleValidator *P_StaticControl_Validator = new QDoubleValidator(-180.0,180.0,4,P_StaticControl_LineEdit);
    QDoubleValidator *I_StaticControl_Validator = new QDoubleValidator(-180.0,180.0,4,I_StaticControl_LineEdit);
    QDoubleValidator *D_StaticControl_Validator = new QDoubleValidator(-180.0,180.0,4,D_StaticControl_LineEdit);

    QDoubleValidator *P_PosControl_Validator = new QDoubleValidator(-180.0,180.0,4,P_PosControl_LineEdit);
    QDoubleValidator *I_PosControl_Validator = new QDoubleValidator(-180.0,180.0,4,I_PosControl_LineEdit);
    QDoubleValidator *D_PosControl_Validator = new QDoubleValidator(-180.0,180.0,4,D_PosControl_LineEdit);

    QDoubleValidator *P_AccControl_Validator = new QDoubleValidator(-180.0,180.0,4,P_AccControl_LineEdit);
    QDoubleValidator *I_AccControl_Validator = new QDoubleValidator(-180.0,180.0,4,I_AccControl_LineEdit);
    QDoubleValidator *D_AccControl_Validator = new QDoubleValidator(-180.0,180.0,4,D_AccControl_LineEdit);

    QDoubleValidator *StaticVoltage_Validator = new QDoubleValidator(-20.0,20.0,4,StaticVoltage_LineEdit);

    P_StaticControl_LineEdit->setValidator(P_StaticControl_Validator);
    I_StaticControl_LineEdit->setValidator(I_StaticControl_Validator);
    D_StaticControl_LineEdit->setValidator(D_StaticControl_Validator);

    P_PosControl_LineEdit->setValidator(P_PosControl_Validator);
    I_PosControl_LineEdit->setValidator(I_PosControl_Validator);
    D_PosControl_LineEdit->setValidator(D_PosControl_Validator);

    P_AccControl_LineEdit->setValidator(P_AccControl_Validator);
    I_AccControl_LineEdit->setValidator(I_AccControl_Validator);
    D_AccControl_LineEdit->setValidator(D_AccControl_Validator);
    StaticVoltage_LineEdit->setValidator(StaticVoltage_Validator);
    PID_RadioButton->setChecked(true);
    PosVariable_RadioButton->setChecked(true);
    SetDefaultParameter();


}

control_param::~control_param()
{
    delete ui;
}

/**
 * @brief control_param::SetDefaultParameter
 * 初始化设置控制参数
 */
void control_param::SetDefaultParameter()
{
    P_StaticControl_LineEdit->setText(QString::number(g_StaticPIDParam.P));
    I_StaticControl_LineEdit->setText(QString::number(g_StaticPIDParam.I));
    D_StaticControl_LineEdit->setText(QString::number(g_StaticPIDParam.D));

    P_PosControl_LineEdit->setText(QString::number(g_PosPIDParam.P));
    I_PosControl_LineEdit->setText(QString::number(g_PosPIDParam.I));
    D_PosControl_LineEdit->setText(QString::number(g_PosPIDParam.D));

    P_AccControl_LineEdit->setText(QString::number(g_AccPIDParam.P));
    I_AccControl_LineEdit->setText(QString::number(g_AccPIDParam.I));
    D_AccControl_LineEdit->setText(QString::number(g_AccPIDParam.D));

    StaticVoltage_LineEdit->setText(QString::number(g_StaticVoltage));

    ControlMethod = g_ControlMethod;
    ControlVariable = g_ControlVariable;
}


void control_param::SetPIDParameter(PIDParamStruct *staticPID, PIDParamStruct *posPID, PIDParamStruct *accPID)
{
    P_StaticControl_LineEdit->setText(QString::number(staticPID->P));
    I_StaticControl_LineEdit->setText(QString::number(staticPID->I));
    D_StaticControl_LineEdit->setText(QString::number(staticPID->D));

    P_PosControl_LineEdit->setText(QString::number(posPID->P));
    I_PosControl_LineEdit->setText(QString::number(posPID->I));
    D_PosControl_LineEdit->setText(QString::number(posPID->D));

    P_AccControl_LineEdit->setText(QString::number(accPID->P));
    I_AccControl_LineEdit->setText(QString::number(accPID->I));
    D_AccControl_LineEdit->setText(QString::number(accPID->D));
}


void control_param::GetPIDParameter(PIDParamStruct *staticPID, PIDParamStruct *posPID, PIDParamStruct *accPID)
{
    staticPID->P = P_StaticControl_LineEdit->text().toDouble();
    staticPID->I = I_StaticControl_LineEdit->text().toDouble();
    staticPID->D = D_StaticControl_LineEdit->text().toDouble();

    posPID->P = P_PosControl_LineEdit->text().toDouble();
    posPID->I = I_PosControl_LineEdit->text().toDouble();
    posPID->D = D_PosControl_LineEdit->text().toDouble();

    accPID->P = P_AccControl_LineEdit->text().toDouble();
    accPID->I = I_AccControl_LineEdit->text().toDouble();
    accPID->I = D_AccControl_LineEdit->text().toDouble();


}

void control_param::GetStaticVoltage(double *staticV)
{
    *staticV = StaticVoltage_LineEdit->text().toDouble();
}

/**
 * @brief control_param::on_PID_RadioButton_clicked
 * 当PID控制按钮按下
 */
void control_param::on_PID_RadioButton_clicked()
{
    ControlMethod  = ControlMethodEnum::PID;
}


void control_param::on_TVC_RadioButton_clicked()
{
    ControlMethod  = ControlMethodEnum::TVC;
}


void control_param::on_ThreePID_RadioButton_clicked()
{
    ControlMethod = ControlMethodEnum::ThreePID;
}



void control_param::on_IterativeControl_RadioButton_clicked()
{
    ControlMethod = ControlMethodEnum::IterativeControl;
}


/**
 * @brief control_param::on_ReadConfigFile_PushButton_clicked
 * 从Ini文件中读取配置文件
 */
void control_param::on_ReadConfigFile_PushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,"打开文件","",
                                                    "INI(*.ini)");
    qDebug()<<"Select file : "<<fileName;

    if(fileName.isEmpty())
    {
        QMessageBox::warning(this,"警告","打开文件错误!",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
        return;
    }
    PIDParamStruct *staticPID = new PIDParamStruct;
    PIDParamStruct *posPID = new PIDParamStruct;
    PIDParamStruct *accPID = new PIDParamStruct;

    if(!myHelper::ReadFromPIDParamIni(fileName,staticPID,posPID,accPID))
    {
        QMessageBox::warning(this,"警告","无法读取参数!",QMessageBox::Ok|QMessageBox::Cancel,QMessageBox::Ok);
        return;
    }
    SetPIDParameter(staticPID,posPID,accPID);
    delete staticPID;
    delete posPID;
    delete accPID;

}


/**
 * @brief control_param::on_OK_PushButton_clicked
 * 参数设置窗口接受按钮被按下
 */
void control_param::on_OK_PushButton_clicked()
{
    //重新设置全局参数
    GetPIDParameter(&g_StaticPIDParam,&g_PosPIDParam,&g_AccPIDParam);
    GetStaticVoltage(&g_StaticVoltage);
    //重新设置控制方法
    g_ControlMethod = ControlMethod;
    g_ControlVariable = ControlVariable;

    //使用服务器发送参数数据包
    //PID控制参数以及控制方法

    //控制方法帧
    g_TcpMsgServer->SendMsgToClient(ProtocolSet::ControlMethod,&g_ControlMethod,sizeof(g_ControlMethod));
    g_TcpMsgServer->SendMsgToClient(ProtocolSet::ControlVarible,&g_ControlVariable,sizeof(g_ControlVariable));
    g_TcpMsgServer->SendMsgToClient(ProtocolSet::StaticVoltage,&g_StaticVoltage,sizeof(g_StaticVoltage));
    g_TcpMsgServer->SendMsgToClient(ProtocolSet::StaticPID,&g_StaticPIDParam,sizeof(g_StaticPIDParam));
    g_TcpMsgServer->SendMsgToClient(ProtocolSet::PosPID,&g_PosPIDParam,sizeof(g_PosPIDParam));
    g_TcpMsgServer->SendMsgToClient(ProtocolSet::AccPID,&g_AccPIDParam,sizeof(g_AccPIDParam));
    this->close();
}

void control_param::on_Cancel_PushButton_clicked()
{
    //
    this->close();
}

void control_param::on_PosVariable_RadioButton_clicked()
{
    ControlVariable = ControlVariableEnum::Pos;
}

void control_param::on_FVariable_RadioButton_clicked()
{
    ControlVariable = ControlVariableEnum::F;
}
