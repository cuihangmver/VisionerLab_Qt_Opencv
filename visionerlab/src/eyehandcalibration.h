#ifndef EYEHANDCALIBRATION_H
#define EYEHANDCALIBRATION_H

#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <opencv2/opencv.hpp>
#include <QDebug>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLineEdit>
#include <QFileDialog>
namespace Ui {
class EyeHandCalibration;
}

class EyeHandCalibration : public QDialog
{
    Q_OBJECT

public:
    explicit EyeHandCalibration(QWidget *parent = nullptr);
    ~EyeHandCalibration();

private:
    Ui::EyeHandCalibration *ui;
    QLabel *pLabel1;
    QLabel *pLabel2;


    QLineEdit* lineEdit1;
    QLineEdit* lineEdit2;


    QDialogButtonBox *button;
    QWidget *m_pParent;

    QHBoxLayout *qButtonHor1;
    QHBoxLayout *qButtonHor2;


    QVBoxLayout *qButtonVer;
    QString qsLineEditText1;
    QString qsLineEditText2;

    QPushButton *m_pOpenTCP;
    QPushButton *m_pOpenEXtrinsicCam;
private:
    void closeEvent(QCloseEvent *event);
private slots:
    void OKCalParam();
    void CancelCalParam();
    void OpenPathTCP();
    void OpenPathECam();
signals:
    void EyeHandCalibrationParamSend(std::string ,std::string);
};
#endif // EYEHANDCALIBRATION_H
