#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFileDevice>
#include <QFile>
#include <QMessageBox>
#include <QtEvents>
#include <QTextStream>
#include <QDebug>
#include <opencv2/opencv.hpp>
#include <QTextCodec>
#include <QPixmap>
#include <QTextEdit>

#include "form.h"
#include <QMimeData>
#include <QDebug>
#include <QListWidget>
#include <QPushButton>

#include "inforbase.h"
#include "centralwidget.h"
//拖拽事件
#include <QDragEnterEvent>
//放下事件
#include <QDropEvent>
#include <iostream>
#include "clog.h"
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public:
    // 自定义函数
    // 显示图像
    void MainWindow::initButton(Ui::MainWindow *ui);
    // void MainWindow::openButton(Ui::MainWindow *ui);
    void getCentralWidget(CentralWidget *cenwidget);
private:

private slots:
    // 槽函数
    void on_actionOpen_triggered();
    void OpenNew();
    // 按钮亮灭管理
    void ButtonShowManage(std::vector<std::string> ,std::vector<std::string>);
private:
    Ui::MainWindow *ui;
    Ui::MainWindow *m_ui;
    INFOR_BASE::sImgInfor m_ImgInfor;
    CentralWidget *c;
    QWidget *m_parent;
// 定义信号，可以与子窗口进行通信
signals:
    void sendImg(cv::Mat, INFOR_BASE::sImgInfor);
    void sendCloseImgWindow();
};

#endif // MAINWINDOW_H
