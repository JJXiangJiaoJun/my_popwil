#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <QString>
#include <QFile>
#include <QFileDialog>
#include <QIODevice>
#include <QTextStream>
#include <QTextCodec>

#include "global_setting.h"
#include "globaldata.h"


class ReportGenerator
{
public:
    ReportGenerator();
    static bool GenerateCSVReport(QString fileName,QString sep);

};

#endif // REPORTGENERATOR_H
