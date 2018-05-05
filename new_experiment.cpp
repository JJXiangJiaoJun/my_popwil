#include "new_experiment.h"
#include "ui_new_experiment.h"

new_experiment::new_experiment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::new_experiment)
{
    ui->setupUi(this);
}

new_experiment::~new_experiment()
{
    delete ui;
}

void new_experiment::on_exit_clicked()
{

}
