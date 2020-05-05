#ifndef ADAPTIVEPHASECONTROL_H
#define ADAPTIVEPHASECONTROL_H

#define ORDER 2

#include <QVector>
#include <cmath>

class AdaptivePhaseControl
{
public:
    AdaptivePhaseControl();
    AdaptivePhaseControl(double fre);
    double getCompensate(double ref,double cur,double T);

private:
    double fundementalFrequency;
    double w[ORDER];
    double learningRate;
    double Update(double error,double T);

};

#endif // ADAPTIVEPHASECONTROL_H
