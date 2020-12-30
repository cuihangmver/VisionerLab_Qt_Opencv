#ifndef CSTRUCTUREDLIGHTCALIBRATION_H
#define CSTRUCTUREDLIGHTCALIBRATION_H

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
class CStructuredLightCalibration;
}

class CStructuredLightCalibration : public QDialog
{
    Q_OBJECT

public:
    explicit CStructuredLightCalibration(QWidget *parent = nullptr);
    ~CStructuredLightCalibration();

private:
    Ui::CStructuredLightCalibration *ui;
    QLabel *pLabel1;
    QLabel *pLabel2;
    QLabel *pLabel3;
    QLabel *pLabel4;
    QLabel *pLabel5;
    QLabel *pLabel6;

    QLineEdit* lineEdit1;
    QLineEdit* lineEdit2;
    QLineEdit* lineEdit3;
    QLineEdit* lineEdit4;
    QLineEdit* lineEdit5;
    QLineEdit* lineEdit6;

    QDialogButtonBox *button;
    QWidget *m_pParent;

    QHBoxLayout *qButtonHor1;
    QHBoxLayout *qButtonHor2;
    QHBoxLayout *qButtonHor3;
    QHBoxLayout *qButtonHor4;
    QHBoxLayout *qButtonHor5;
    QHBoxLayout *qButtonHor6;

    QVBoxLayout *qButtonVer;
    QString qsLineEditText1;
    QString qsLineEditText2;
    QString qsLineEditText3;
    QString qsLineEditText4;
    QString qsLineEditText5;
    QString qsLineEditText6;
    QPushButton *m_pOpenPath;
private:
    void closeEvent(QCloseEvent *event);
private slots:
    void OKCalParam();
    void CancelCalParam();
    void OpenPath();
signals:
    void Structured_LightParamSend(std::string ,cv::Point ,double ,double ,double);
};

#endif // CSTRUCTUREDLIGHTCALIBRATION_H
