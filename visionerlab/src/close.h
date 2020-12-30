#ifndef CLOSE_H
#define CLOSE_H

#include <QDialog>
#include <QSpinBox>
#include <QSlider>
#include <QBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <opencv2/opencv.hpp>
#include <QComboBox>

namespace Ui {
class Close;
}

class Close : public QDialog
{
    Q_OBJECT

public:
    explicit Close(QWidget *parent = nullptr);
    ~Close();
public:
    void ManualThresholdChangeSlot(int nValue);
private:
    Ui::Close *ui;
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
    void ManualCloseChangeSlot(int );
    void SetStructureType(QString);
signals:
    void sendManualCloseChange(int ,QString);
private:

};

#endif // CLOSE_H
