#ifndef CENTRALWIDGET_H
#define CENTRALWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QBoxLayout>
#include <QTextEdit>
#include "inforbase.h"
#include <opencv2/opencv.hpp>
#include <QTextCodec>
#include "form.h"
#include <QMimeData>

namespace Ui {
class CentralWidget;
}

class CentralWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CentralWidget(QWidget *parent = nullptr);
    ~CentralWidget();
    CentralWidget(int a, QWidget *parent);

protected:
    void dragEnterEvent(QDragEnterEvent *event);
    void dropEvent(QDropEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
private:
    Ui::CentralWidget *ui;
    INFOR_BASE::sImgInfor m_ImgInfor;

    QWidget *m_parentCopy;
    QTextEdit *m_textedit;
    Form *m_openimg;

    QHBoxLayout *qButtonHor;
    QVBoxLayout *qButtonVer;
    QPushButton *btn1;
    QPushButton *btn2;
    QPushButton *btn3;
    Form *openimg;
private slots:
    void getMouse(int nMouseX, int nMouseY, QColor color, int nChannel);
    void OpenNew(cv::Mat, INFOR_BASE::sImgInfor);
    void RGB2Gray();
    void Gray2RGB();
    void closeEventSlot();
    void closeEventSaveAsSlot();
    void ManualThresholdSlot();
    void ButtonShowManageCloseGraySlot(std::vector<std::string> ,std::vector<std::string>);
    void ButtonShowManageOpenGraySlot(std::vector<std::string> ,std::vector<std::string>);
// 定义信号，可以与子窗口进行通信
signals:
    void sendImgCenter(cv::Mat, INFOR_BASE::sImgInfor);
    void sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>);
    void sendRGB2Gray();
    void sendGray2RGB();
    void sendcloseEventSlot();
    void sendcloseEventSaveAsSlot();
    void sendManualThresholdSlot();
};

#endif // CENTRALWIDGET_H
