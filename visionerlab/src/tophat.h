#ifndef TOPHAT_H
#define TOPHAT_H

#include <QDialog>
#include <QSpinBox>
#include <QSlider>
#include <QBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <opencv2/opencv.hpp>
#include <QComboBox>

namespace Ui {
class TopHat;
}

class TopHat : public QDialog
{
    Q_OBJECT

public:
    explicit TopHat(QWidget *parent = nullptr);
    ~TopHat();
public:
    void ManualThresholdChangeSlot(int nValue);
private:
    Ui::TopHat *ui;
    QWidget *m_parent;
    QSpinBox *m_pSpinBox;
    QSlider *m_pSlider;
    QLabel *m_pLabel;
    QVBoxLayout *m_qButtonVer;
    QHBoxLayout *m_qSliderHor1;
    QHBoxLayout *m_qSliderHor2;
    QHBoxLayout *m_qSliderHor3;
    QDialogButtonBox *button;

    QLabel *pLabel1;
    QLabel *pLabel2;
    QLabel *pLabel3;
    QString m_StructureType;
    QComboBox *pComboBox1;

private slots:
    void ManualTopHatChangeSlot(int );
    void SetStructureType(QString);
signals:
    void sendManualTopHatChange(int ,QString);
private:

};

#endif // TOPHAT_H
