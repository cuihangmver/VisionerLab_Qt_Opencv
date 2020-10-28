#ifndef DIALOGSLIDER_H
#define DIALOGSLIDER_H

#include <QDialog>
#include <QSpinBox>
#include <QSlider>
#include <QBoxLayout>
#include <QTextEdit>
#include <QLabel>
#include <QCheckBox>
#include <QDialogButtonBox>
#include <QPushButton>
namespace Ui {
class DialogSlider;
}

class DialogSlider : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSlider(QWidget *parent = nullptr);
    ~DialogSlider();
private slots:
    void ManualThresholdChangeSlot(int nValue);
private:
    Ui::DialogSlider *ui;
    QWidget *m_parent;

    QVBoxLayout *qButtonVer;
    QHBoxLayout *qSliderHor1;
    QHBoxLayout *qSliderHor2;
    QHBoxLayout *qSliderHor3;
    QHBoxLayout *qSliderHor4;
    QSpinBox *pSpinBox;
    QSlider *pSlider;
    QLabel *pLabel1;
    QLabel *pLabel2;
    QLabel *pLabel3;
    QLabel *pLabel4;
    QCheckBox *pCheckBox1;
    QCheckBox *pCheckBox2;
    QPushButton *button1;
    QPushButton *button2;
    QDialogButtonBox *button;
signals:
    void sendManualThresholdChange(int );
};

#endif // DIALOGSLIDER_H
