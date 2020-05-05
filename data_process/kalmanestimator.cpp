#include "kalmanestimator.h"

KalmanEstimator::KalmanEstimator()
{

}


KalmanEstimator::KalmanEstimator(double _dt, double _Q_pos = 0.05 , double _Q_vel = 0.01,double _R_pos = 0.5):
dt(_dt),Q_pos(_Q_pos),Q_vel(_Q_vel),R_pos(_R_pos)
{
    //初始化状态以及协方差矩阵
    pos_est = 0.0;
    vel_est = 0.0;
    pos_pred = 0.0;
    vel_pred = 0.0;

    for(int i = 0 ;i< ESTIMATOR_ORDER;i++)
        for(int j = 0;j<ESTIMATOR_ORDER;j++)
        {
            P_prior[i][j] = ((double)rand() % 100 / 100);
            P_post[i][j] =  ((double)rand() % 100 / 100);
        }
}



void KalmanEstimator::Predict(double cur_acc)
{
    //计算预测的状态
    pos_pred = pos_est + dt * vel_est;
    vel_pred = vel_est + dt * cur_acc;

    //计算预测的协方差矩阵
    P_prior[0][0] = P_post[0][0] + dt*(P_post[1][0] + P_post[0][1]) + dt*dt*P_post[1][1] + Q_pos;
    P_prior[0][1] = P_post[0][1] + dt*P_post[1][1];
    P_prior[1][0] = P_post[1][0] + dt*P_post[1][1];
    P_prior[1][1] = P_post[1][1] + Q_vel;

}

void KalmanEstimator::Update(double cur_pos)
{
    double PH_T0 = P_prior[0][0];
    double PH_T1 = P_prior[1][0];

    double E = P_prior[0][0] + R_pos;

    //计算卡尔曼增益
    K[0] = PH_T0 / E;
    K[1] = PH_T1 / E;

    //更新协方差矩阵
    P_post[0][0] = P_prior[0][0] - K[0] * P_prior[0][0];
    P_post[0][1] = P_prior[0][1] - K[0] * P_prior[0][1];
    P_post[1][0] = P_prior[1][0] - K[1] * P_prior[0][0];
    P_post[1][1] = P_prior[1][1] - K[1] * P_prior[0][1];

    //更新状态，状态估计
    error = cur_pos - pos_pred;
    pos_est = pos_pred + K[0] * error;
    vel_est = vel_pred  + K[1] * error;

    //return vel_est;

}

double KalmanEstimator::Filter(double cur_acc, double cur_pos)
{
    Predict(cur_acc);
    Update(cur_pos);
    return vel_est;
}
