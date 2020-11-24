#include "gaussiandialog.h"
#include "ui_gaussiandialog.h"

GaussianDialog::GaussianDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GaussianDialog)
{
    ui->setupUi(this);

}

GaussianDialog::~GaussianDialog()
{
    delete ui;
}
