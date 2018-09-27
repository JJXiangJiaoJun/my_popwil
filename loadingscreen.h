#ifndef LOADINGSCREEN_H
#define LOADINGSCREEN_H

#include <QObject>
#include <QWidget>
#include <QSplashScreen>

class loadingscreen : public QSplashScreen
{
public:
    loadingscreen(QWidget *parent=0);
};

#endif // LOADINGSCREEN_H
