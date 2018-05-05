#ifndef NEW_EXPERIMENT_H
#define NEW_EXPERIMENT_H

#include <QDialog>

namespace Ui {
class new_experiment;
}

class new_experiment : public QDialog
{
    Q_OBJECT

public:
    explicit new_experiment(QWidget *parent = 0);
    ~new_experiment();

private slots:
    void on_exit_clicked();

private:
    Ui::new_experiment *ui;
};

#endif // NEW_EXPERIMENT_H
