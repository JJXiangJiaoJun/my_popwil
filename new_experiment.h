#ifndef NEW_EXPERIMENT_H
#define NEW_EXPERIMENT_H

#include <QDialog>
#include <QFile>
#include <QDataStream>
#include <QByteArray>
#include <QString>
#include "sinewave.h"

namespace Ui {
class new_experiment;
}

class new_experiment : public QDialog
{
    Q_OBJECT

public:
    explicit new_experiment(QWidget *parent = 0);
    ~new_experiment();

private slots:
    void on_exit_clicked();

    void on_sin_clicked();

    void get_sine_wave_para(QString A,QString f,QString t,QString p,QString samp);

    void on_SineWaveExperiment_Button_clicked();

private:
    Ui::new_experiment *ui;
    void generate_sine_wave_data(QString filename,float m_amplitude,float m_frequency,float m_time,float m_phase);

private:
    sinewave sinepara;
    QString filename;
    float max_time;
    float sample_rate;
    float amplitude;
    float frequecy;
    float phase;
    float time;
};



class integrator{
public:
    void Func_Integral_Trapezoid(QString filename,int n,float *data,int interval);

};





#endif // NEW_EXPERIMENT_H
