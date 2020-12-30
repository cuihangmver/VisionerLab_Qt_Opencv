#ifndef BLACKHAT_H
#define BLACKHAT_H

#include <QDialog>
#include <QSpinBox>
#include <QSlider>
#include <QBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <opencv2/opencv.hpp>
#include <QComboBox>

namespace Ui {
class BlackHat;
}

class BlackHat : public QDialog
{
    Q_OBJECT

public:
    explicit BlackHat(QWidget *parent = nullptr);
    ~BlackHat();
public:
    void ManualThresholdChangeSlot(int nValue);
private:
    Ui::BlackHat *ui;
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
private:
    void closeEvent(QCloseEvent *event);
private slots:
    void ManualBlackHatChangeSlot(int );
    void SetStructureType(QString);
signals:
    void sendManualBlackHatChange(int ,QString);
private:

};
#endif // BLACKHAT_H
