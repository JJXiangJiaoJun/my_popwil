#ifndef KALMANESTIMATOR_H
#define KALMANESTIMATOR_H

#include <QVector>
#include <cmath>

#define ESTIMATOR_ORDER 2

class KalmanEstimator
{
public:
    KalmanEstimator();
    KalmanEstimator(double _dt,double _Q_pos = 0.05,double _Q_vel = 0.01,double _R_pos = 0.5);
    double Filter(double cur_acc,double cur_pos);           //对速度进行状态估计
private:
    void Predict(double cur_acc);
    void Update(double cur_pos);

    double Q_pos;       //位移过程噪声
    double Q_vel;       //速度过程噪声
    double R_pos;       //位移测量噪声
    double dt;          //滤波器采样时间
    double P_prior[ESTIMATOR_ORDER][ESTIMATOR_ORDER];
    double P_post[ESTIMATOR_ORDER][ESTIMATOR_ORDER];
    double K[ESTIMATOR_ORDER];         //卡尔曼滤波增益
    double pos_pred;
    double vel_pred;
    double pos_est;
    double vel_est;
};

#endif // KALMANESTIMATOR_H
