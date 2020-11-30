#ifndef LAPLACE_H
#define LAPLACE_H

#include <QDialog>
#include <QSpinBox>
#include <QSlider>
#include <QBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <opencv2/opencv.hpp>
namespace Ui {
class Laplace;
}

class Laplace : public QDialog
{
    Q_OBJECT

public:
    explicit Laplace(QWidget *parent = nullptr);
    ~Laplace();
public:
    void ManualThresholdChangeSlot(int nValue);
private:
    Ui::Laplace *ui;
    QWidget *m_parent;
    QSpinBox *m_pSpinBox;
    QSlider *m_pSlider;
    QLabel *m_pLabel;
    QVBoxLayout *m_qButtonVer;
    QHBoxLayout *m_qSliderHor1;
    QHBoxLayout *m_qSliderHor2;
    QDialogButtonBox *button;
private slots:
    void ManualLaplaceChangeSlot(int );
signals:
    void sendManualLaplaceChange(int );
};

#endif // LAPLACE_H
