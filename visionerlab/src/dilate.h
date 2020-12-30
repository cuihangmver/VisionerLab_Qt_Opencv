#ifndef DILATE_H
#define DILATE_H

#include <QDialog>
#include <QDialog>
#include <QSpinBox>
#include <QSlider>
#include <QBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <opencv2/opencv.hpp>
#include <QComboBox>
namespace Ui {
class Dilate;
}

class Dilate : public QDialog
{
    Q_OBJECT

public:
    explicit Dilate(QWidget *parent = nullptr);
    ~Dilate();
public:
    void ManualThresholdChangeSlot(int nValue);
private:
    Ui::Dilate *ui;
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
    void ManualDilateChangeSlot(int );
    void SetStructureType(QString);
signals:
    void sendManualDilateChange(int ,QString);
private:

};

#endif // DILATE_H
