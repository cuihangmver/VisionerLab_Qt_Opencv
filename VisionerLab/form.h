#ifndef FORM_H
#define FORM_H

#include <QWidget>
#include <QMainWindow>
#include <QDialog>
#include <QDebug>
#include <QPushButton.h>
#include <QboxLayout.h>
#include <QMouseEvent>
#include <QLabel>
#include <QScrollArea>
#include <QPainter>
#include <opencv2/opencv.hpp>
#include <QMessageBox>
#include <QFileDialog>
#include "inforbase.h"
#include "dialogslider.h"
#include "adaptivedialog.h"
#include <QTime>

namespace Ui {
class Form;
}
// 调用centralwidget类，防止循环引用
class CentralWidget;
class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);

    ~Form();
    // 自定义函数
    void DisplayMat(cv::Mat mimage, QLabel *label, double dScaling);
    void Display4DepthMat(cv::Mat image, QImage &img);
    void Display3DepthMat(cv::Mat image, QImage &img);
    void Display2DepthMat(cv::Mat image, QImage &img);
    void Display1DepthMat(cv::Mat image, QImage &img);
    void Resize(cv::Mat mSrc, cv::Mat &mDst, double dScale);
    void doFileSave();
protected:
    // 事件相应
    void mousePressEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *ev);
    void keyReleaseEvent(QKeyEvent *ev);
    void closeEvent(QCloseEvent *event);
private:
    Ui::Form *ui;
    QWidget *m_parentCopy;
    QLabel *m_label;
    cv::Mat m_mImg;
    cv::Mat m_mTem;
    INFOR_BASE::sImgInfor m_imgInfor;
    double m_dScaling;
    // 滚动条
    QScrollArea *scrollArea;
    bool m_bCtrlPress;
    bool m_bModifyImg;
    DialogSlider *m_dialogSlider;
    AdaptiveDialog *m_pAdaptiveDialog;
    QHBoxLayout *pg;
    QVBoxLayout *pv;
    QPushButton *pb,*pb1,*pb2;
private slots:
    // 槽函数
    // 接收主窗口的值
    // void getImg(cv::Mat, INFOR_BASE::sImgInfor);
    void getImgCenter(cv::Mat, INFOR_BASE::sImgInfor);
    void closeEventSlot();
    void closeEventSaveAsSlot();
    // 接收子窗口数值
    void OKSelectImg();
    void CancelSelectImg();
    // 图像处理槽函数
    void RGB2Gray();
    void Gray2RGB();
    void ManualThresholdSlot();
    void ManualThresholdChangeSlot(int );
    void ThresholdOtusSlot();
    void ThresholdAdaptiveSlot();
    void EqualizationSlot();
    void SobelSlot();
    void Scharr_3_3Slot();
    void Laplacian_5_5Slot();
    void Laplacian_7_7Slot();
    void Gaussian_3_3Slot();
    void Gaussian_5_5Slot();
    void Gaussian_7_7Slot();
    void Mean_3_3Slot();
    void Mean_5_5Slot();
    void Mean_7_7Slot();
    void Median_3_3Slot();
    void Median_5_5Slot();
    void Median_7_7Slot();
    void ThresholdAdaptiveChangeSlot(double dMaxValue, int nAdaptiveMethod, int nThresholdType, int nBlockSize, double dC);
    void PreviewThresholdAdaptiveChangeSlot(double dMaxValue, int nAdaptiveMethod, int nThresholdType, int nBlockSize, double dC);

signals:
    // 信号函数
    void sendMouse(int, int, QColor, int );
    void sendMouseDepth(int, int, ushort, int );
    void sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>);
    void sendCloseImgWindow();
};

#endif // FORM_H
