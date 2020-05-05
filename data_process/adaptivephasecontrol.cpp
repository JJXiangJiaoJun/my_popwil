#include "adaptivephasecontrol.h"

const double pi = acos(-1);

AdaptivePhaseControl::AdaptivePhaseControl()
{

}

AdaptivePhaseControl::AdaptivePhaseControl(double fre)
{
    fundementalFrequency = fre;
    for(int i = 0;i < ORDER ;i++)
        w[i] = double(rand()%100)/100;
}

double AdaptivePhaseControl::getCompensate(double ref,double cur,double T)
{
    double e_k = ref - cur;
    Update(e_k,T);
    double output = w[0] * sin(2*pi*fundementalFrequency*T) + w[1] * sin(2*pi*fundementalFrequency*T);
    return output;
}


double AdaptivePhaseControl::Update(double error,double T)
{
    w[0] = w[0] + learningRate * sin(2*pi*fundementalFrequency*T)*error;
    w[1] = w[1] + learningRate * cos(2*pi*fundementalFrequency*T)*error;
}
