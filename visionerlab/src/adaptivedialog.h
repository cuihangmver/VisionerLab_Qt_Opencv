#ifndef ADAPTIVEDIALOG_H
#define ADAPTIVEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QDialogButtonBox>
#include <QPushButton>
namespace Ui {
class AdaptiveDialog;
}

class AdaptiveDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AdaptiveDialog(QWidget *parent = nullptr);
    ~AdaptiveDialog();

private:
    Ui::AdaptiveDialog *ui;
    QLabel *pLabel1;
    QLabel *pLabel2;
    QLabel *pLabel3;
    QLabel *pLabel4;
    QLabel *pLabel5;

    QComboBox *pComboBox1;
    QComboBox *pComboBox2;
    QComboBox *pComboBox3;
    QComboBox *pComboBox4;
    QComboBox *pComboBox5;

    QHBoxLayout *qButtonHor1;
    QHBoxLayout *qButtonHor2;
    QHBoxLayout *qButtonHor3;
    QHBoxLayout *qButtonHor4;
    QHBoxLayout *qButtonHor5;

    QVBoxLayout *qButtonVer;
    QDialogButtonBox *button;

    QPushButton *pb;
private:
    double m_dMaxValue;
    int m_nAdaptiveMethod;
    int m_nThresholdType;
    int m_nBlockSize;
    double m_dC;
    QWidget *m_pParent;
private:
    // 事件函数
    void closeEvent(QCloseEvent *event);
private slots:
    void SetMaxValue(QString sMaxValue);
    void SetAdaptiveMethod(QString sAdaptiveMethod);
    void SetThresholdType(QString sThresholdType);
    void SetBlockSize(QString sBlockSize);
    void SetC(QString sC);
    void OKSelectAdaptive();
    void CancelSelectAdaptive();
    void PreviewSelectAdaptive();
signals:
    void ThresholdAdaptiveChangeSend(double , int , int , int , double );
    void PreviewThresholdAdaptiveChangeSend(double , int , int , int , double );
};

#endif // ADAPTIVEDIALOG_H
