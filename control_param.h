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

#include "globaldata.h"
#include "myhelper.h"
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

    QRadioButton *PID_RadioButton;
    QRadioButton *TVC_RadioButton;
    QRadioButton *ThreePID_RadioButton;
    QRadioButton *IterativeControl_RadioButton;

    void SetDefaultParameter();
    void SetPIDParameter(PIDParamStruct *staticPID,
                         PIDParamStruct *posPID, PIDParamStruct *accPID);
    void GetPIDParameter(PIDParamStruct *staticPID,
                         PIDParamStruct *posPID, PIDParamStruct *accPID);
private slots:

    void on_PID_RadioButton_clicked();
    void on_TVC_RadioButton_clicked();
    void on_ThreePID_RadioButton_clicked();
    void on_IterativeControl_RadioButton_clicked();
    void on_ReadConfigFile_PushButton_clicked();
    void on_OK_PushButton_clicked();
    void on_Cancel_PushButton_clicked();
};

#endif // CONTROL_PARAM_H
