#ifndef SINEWAVE_H
#define SINEWAVE_H

#include <QDialog>
#include <QString>
#include <QLineEdit>


#include "global_setting.h"
#include "globaldata.h"


namespace Ui {
class sinewave;
}

class sinewave : public QDialog
{
    Q_OBJECT

public:
    explicit sinewave(QWidget *parent = 0);
    ~sinewave();

private slots:
    void on_generate_wave_clicked();

    void on_cancel_clicked();

private:
    Ui::sinewave *ui;
signals:
void sinewave_para(QString A,QString f,QString t,QString p,QString samp);


public:
    //保存正弦波动参数
    QString ampilitude_text;
    QString frequency_text;
    QString time_text;
    QString phase_text;
    QString samplerate_text;

private:
    QLineEdit *Amplitude_LineEdit;
    QLineEdit *Frequency_LineEdit;

};

#endif // SINEWAVE_H
