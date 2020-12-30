#ifndef CONNECTEDREGION_H
#define CONNECTEDREGION_H

#include <QDialog>
#include <QSpinBox>
#include <QSlider>
#include <QBoxLayout>
#include <QLabel>
#include <QDialogButtonBox>
#include <QComboBox>
#include <QDebug>
namespace Ui {
class ConnectedRegion;
}

class ConnectedRegion : public QDialog
{
    Q_OBJECT

public:
    explicit ConnectedRegion(QWidget *parent = nullptr, int nRegion = 0);
    ~ConnectedRegion();


private:
    Ui::ConnectedRegion *ui;
    QLabel *pLabel1;
    QComboBox *pComboBox1;
    QWidget *m_parent;
    QSpinBox *m_pSpinBox;
    QSlider *m_pSlider;
    QLabel *m_pLabel;
    QVBoxLayout *m_qButtonVer;
    QHBoxLayout *m_qSliderHor1;
    QHBoxLayout *m_qSliderHor2;
    QHBoxLayout *m_qSliderHor3;
    QDialogButtonBox *button;

    double m_nConnectedComponents48Value;
private:
    void closeEvent(QCloseEvent *event);
private slots:
    void ManualConnectedRegionChangeSlot(int );
    void SetConnectedComponents48Slot(QString);
    //void OKConnected_RegionSliderSelectImg();
    //void CancelConnected_RegionSliderSelectImg();
signals:
    void sendManualConnectedRegionChange(int ,int);
    //void sendOKConnected_RegionSliderSelectImg();
};

#endif // CONNECTEDREGION_H
