#ifndef GAUSSIANDIALOG_H
#define GAUSSIANDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QSlider>
#include <QBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <opencv2/opencv.hpp>
namespace Ui {
class GaussianDialog;
}

class GaussianDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GaussianDialog(QWidget *parent = nullptr);
    ~GaussianDialog();
public:
    void ManualThresholdChangeSlot(int nValue);
private:
    Ui::GaussianDialog *ui;
    QWidget *m_parent;
    QSpinBox *m_pSpinBox;
    QSlider *m_pSlider;
    QLabel *m_pLabel;
    QVBoxLayout *m_qButtonVer;
    QHBoxLayout *m_qSliderHor1;
    QHBoxLayout *m_qSliderHor2;
    QDialogButtonBox *button;
private slots:
    void ManualGaussianChangeSlot(int );
signals:
    void sendManualGaussianChange(int );
};

#endif // GAUSSIANDIALOG_H
