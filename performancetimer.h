#ifndef PERFORMANCETIMER_H
#define PERFORMANCETIMER_H
#include <QObject>
#include <windows.h>
class PerformanceTimer : public QObject
{
    Q_OBJECT
public:
    explicit PerformanceTimer(QObject *parent = 0);
    ~PerformanceTimer();
signals:
    void timeout();
public slots:
    void start(int timeInterval);
    void stop();
    friend WINAPI void  CALLBACK PeriodCycle(uint,uint,DWORD_PTR,DWORD_PTR,DWORD_PTR);
private:
    int m_interval;
    int m_connectId;
};
#endif // PERFORMANCETIMER_H
