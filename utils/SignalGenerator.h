#ifndef SIGNALGENERATOR_H
#define SIGNALGENERATOR_H

#include <cmath>
#include <cstdlib>
#include <ctime>
#include <QFile>
#include <QTextStream>
#include <QString>

#include "globaldata.h"

#define random(x) (rand()%x)
/**
 * @brief The SignalGenerator class
 */
class SignalGenerator
{
public:
    SignalGenerator();
    static void GenerateSineWave(SineWaveParamStruct param);
    static void GenerateRandomWave(double amplitude);
    static void GenerateTriangleWave(SineWaveParamStruct param);
    static void GenerateSineSweepWave(QString path);
};

#endif // SIGNALGENERATOR_H
