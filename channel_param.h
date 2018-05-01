#ifndef CHANNEL_PARAM_H
#define CHANNEL_PARAM_H

#include <QWidget>

namespace Ui {
class channel_param;
}

class channel_param : public QWidget
{
    Q_OBJECT

public:
    explicit channel_param(QWidget *parent = 0);
    ~channel_param();

private:
    Ui::channel_param *ui;
};

#endif // CHANNEL_PARAM_H
