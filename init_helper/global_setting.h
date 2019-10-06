#ifndef GLOBAL_SETTING_H
#define GLOBAL_SETTING_H

#include <QtGlobal>


typedef double ChartDataType;
typedef quint16 FrameLengthType;
typedef qint16  FrameFuncType;
typedef double  PosPeakValueType;
typedef double AccPeakValueType;




struct ExperimentParamStruct
{
    qint16 waveform;
    double amplitude;
    double frequency;
};


class GlobalSetting
{
public:
    GlobalSetting();
};

#endif // GLOBAL_SETTING_H
