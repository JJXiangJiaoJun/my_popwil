#ifndef CONTROL_PARAM_H
#define CONTROL_PARAM_H

#include <QWidget>
#include <QLineEdit>
#include <QRadioButton>
#include <QDoubleValidator>
#include <QString>
#include <QFileDialog>
#include <QDebug>
#include <QMessageBox>
#include <QtGlobal>

#include "global_setting.h"
#include "globaldata.h"
#include "myhelper.h"
#include "protocol.h"
#include "tcpserver.h"

namespace Ui {
class control_param;
}

class control_param : public QWidget
{
    Q_OBJECT

public:
    explicit control_param(QWidget *parent = 0);
    ~control_param();

private:
    qint16 ControlMethod;
    qint16 ControlVariable;
    double StaticVoltage;

    Ui::control_param *ui;
    QLineEdit *P_StaticControl_LineEdit;
    QLineEdit *I_StaticControl_LineEdit;
    QLineEdit *D_StaticControl_LineEdit;

    QLineEdit *P_PosControl_LineEdit;
    QLineEdit *I_PosControl_LineEdit;
    QLineEdit *D_PosControl_LineEdit;

    QLineEdit *P_AccControl_LineEdit;
    QLineEdit *I_AccControl_LineEdit;
    QLineEdit *D_AccControl_LineEdit;

    QLineEdit *StaticVoltage_LineEdit;

    QRadioButton *PID_RadioButton;
    QRadioButton *TVC_RadioButton;
    QRadioButton *ThreePID_RadioButton;
    QRadioButton *IterativeControl_RadioButton;
    QRadioButton *PosVariable_RadioButton;
    QRadioButton *FVariable_RadioButton;


    void SetDefaultParameter();
    void SetPIDParameter(PIDParamStruct *staticPID,
                         PIDParamStruct *posPID, PIDParamStruct *accPID);
    void GetPIDParameter(PIDParamStruct *staticPID,
                         PIDParamStruct *posPID, PIDParamStruct *accPID);
    void SetControlMethod(qint16 ctrlMethod);
    void SetControlVaribale(qint16 ctrlVariable);
    void GetStaticVoltage(double *staticV);
    void SetStaticVoltage(double *staticV);
private slots:

    void on_PID_RadioButton_clicked();
    void on_TVC_RadioButton_clicked();
    void on_ThreePID_RadioButton_clicked();
    void on_IterativeControl_RadioButton_clicked();
    void on_ReadConfigFile_PushButton_clicked();
    void on_OK_PushButton_clicked();
    void on_Cancel_PushButton_clicked();
    void on_PosVariable_RadioButton_clicked();
    void on_FVariable_RadioButton_clicked();
    void on_WriteConfigFile_PushButton_clicked();
    void on_SetDefaultConfig_PushButton_clicked();
};

#endif // CONTROL_PARAM_H
