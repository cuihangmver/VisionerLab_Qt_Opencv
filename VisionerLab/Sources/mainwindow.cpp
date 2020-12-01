#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 主界面对象
    c = new CentralWidget(1, this);
    setCentralWidget(c);
    // button的信号是clicked(),QAction的信号是triggered()
    QObject::connect(ui->actionNew, SIGNAL(triggered()), this,SLOT(OpenNew()));
    // 图像处理槽函数
    // 在此添加图像处理信号...
    connect(ui->actionGray, SIGNAL(triggered()), c, SLOT(RGB2Gray()));
    connect(ui->actionRGB, SIGNAL(triggered()), c, SLOT(Gray2RGB()));
    connect(ui->actionsave, SIGNAL(triggered()), c,SLOT(closeEventSlot()));
    connect(ui->actionsave_2, SIGNAL(triggered()), c,SLOT(closeEventSaveAsSlot()));
    connect(ui->actionManual, SIGNAL(triggered()), c,SLOT(ManualThresholdSlot()));
    connect(ui->actionOtus, SIGNAL(triggered()), c,SLOT(ThresholdOtusSlot()));
    connect(ui->actionAdaptive, SIGNAL(triggered()), c,SLOT(ThresholdAdaptiveSlot()));
    connect(ui->actionEqualization, SIGNAL(triggered()), c,SLOT(EqualizationSlot()));
    connect(ui->actionSobel, SIGNAL(triggered()), c,SLOT(SobelSlot()));
    connect(ui->actionScharr_3_3, SIGNAL(triggered()), c,SLOT(Scharr_3_3Slot()));
    connect(ui->actionLaplacian_5_5, SIGNAL(triggered()), c,SLOT(Laplacian_5_5Slot()));
    connect(ui->actionLaplacian_7_7, SIGNAL(triggered()), c,SLOT(Laplacian_7_7Slot()));
    connect(ui->actionGaussian_3_3, SIGNAL(triggered()), c,SLOT(Gaussian_3_3Slot()));
    connect(ui->actionGaussian_5_5, SIGNAL(triggered()), c,SLOT(Gaussian_5_5Slot()));
    connect(ui->actionGaussian_7_7, SIGNAL(triggered()), c,SLOT(Gaussian_7_7Slot()));
    connect(ui->actionMean_3_3, SIGNAL(triggered()), c,SLOT(Mean_3_3Slot()));
    connect(ui->actionMean_5_5, SIGNAL(triggered()), c,SLOT(Mean_5_5Slot()));
    connect(ui->actionMean_7_7, SIGNAL(triggered()), c,SLOT(Mean_7_7Slot()));
    connect(ui->actionMedian_3_3, SIGNAL(triggered()), c,SLOT(Median_3_3Slot()));
    connect(ui->actionMedian_5_5, SIGNAL(triggered()), c,SLOT(Median_5_5Slot()));
    connect(ui->actionMedian_7_7, SIGNAL(triggered()), c,SLOT(Median_7_7Slot()));
    connect(ui->actionGaussian, SIGNAL(triggered()), c,SLOT(GaussianSlot()));
    connect(ui->actionLaplace, SIGNAL(triggered()), c,SLOT(LaplaceSlot()));
    connect(ui->actionConnected_Region, SIGNAL(triggered()), c,SLOT(Connected_RegionSlot()));
    initButton(ui);
    m_ui = ui;
    m_parent = parent;
}

MainWindow::~MainWindow()
{
    if(nullptr != c)
    {
        delete c;
        c = nullptr;
    }
    if(nullptr != m_parent)
    {
        delete m_parent;
        m_parent = nullptr;
    }
    if(nullptr != ui)
    {
        delete ui;
        ui = nullptr;
    }
}
void MainWindow::getCentralWidget(CentralWidget *cenwidget)
{
    cenwidget = c;
}
void MainWindow::initButton(Ui::MainWindow *ui)
{
    ui->actionRGB->setDisabled(true);
    ui->actionGray->setDisabled(true);
    ui->actionMap->setDisabled(true);
    ui->actionsave->setDisabled(true);
    ui->actionsave_2->setDisabled(true);
    ui->actionBinary->setDisabled(true);
    ui->actionManual->setDisabled(true);
    ui->actionOtus->setDisabled(true);
    ui->actionAdaptive->setDisabled(true);
    ui->actionEqualization->setDisabled(true);
    ui->actionExpansion->setDisabled(true);
    ui->menuConvolve_Kernel->setDisabled(true);
}


void MainWindow::openButton(Ui::MainWindow *ui)
{
    ui->actionRGB->setDisabled(false);
    ui->actionGray->setDisabled(false);
    ui->actionMap->setDisabled(false);
    ui->actionsave->setDisabled(false);
    ui->actionsave_2->setDisabled(false);
    ui->actionBinary->setDisabled(false);
}

void MainWindow::OpenNew()
{
    // 打开图像
    QString fileNameImg = QFileDialog::getOpenFileName(this, tr("Open Image"),"./",tr("Image File (*.jpg *.png *.bmp)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(fileNameImg).data();//filename.toAscii().data()

    cv::Mat image = cv::imread(name, -1);
    if(!image.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Image Data Is Null"));
        msgBox.exec();
    }
    else
    {
        m_ImgInfor.sPath = name;
        m_ImgInfor.nWidth = image.rows;
        m_ImgInfor.nHeight = image.cols;
        m_ImgInfor.sColorSpace = "RGB";
        connect(this,SIGNAL(sendImg(cv::Mat, INFOR_BASE::sImgInfor)),c,SLOT(OpenNew(cv::Mat, INFOR_BASE::sImgInfor)));
        emit sendImg(image, m_ImgInfor);
        disconnect(this,SIGNAL(sendImg(cv::Mat, INFOR_BASE::sImgInfor)),c,SLOT(OpenNew(cv::Mat, INFOR_BASE::sImgInfor)));
    }
}

void MainWindow::ButtonShowManage(std::vector<std::string> vsShowButtons, std::vector<std::string> vsCloseButtons)
{
    // 将所有图像处理按钮关闭

    std::string s1 = "actionRGB";
    std::string s2 = "actionGray";
    std::string s3 = "actionMap";
    std::string s4 = "actionsave";
    std::string s5 = "actionsave_2";
    std::string s6 = "actionBinary";
    std::string s7 = "actionManual";
    std::string s8 = "actionOtus";
    std::string s9 = "actionAdaptive";
    std::string s10 = "actionEqualization";
    std::string s11 = "menuConvolve_Kernel";

    for(int i = 0; i < vsShowButtons.size(); i++)
    {
        QString qstr2 = QString::fromStdString(vsShowButtons[i]);
        if(s1 == vsShowButtons[i])
        {
            ui->actionRGB->setDisabled(false);
        }
        else if(s2 == vsShowButtons[i])
        {
            ui->actionGray->setDisabled(false);
        }
        else if(s3 == vsShowButtons[i])
        {
            ui->actionMap->setDisabled(false);
        }
        else if(s4 == vsShowButtons[i])
        {
            ui->actionsave->setDisabled(false);
        }
        else if(s5 == vsShowButtons[i])
        {
            ui->actionsave_2->setDisabled(false);
        }
        else if(s6 == vsShowButtons[i])
        {
            ui->actionBinary->setDisabled(false);
        }
        else if(s7 == vsShowButtons[i])
        {
            ui->actionManual->setDisabled(false);
        }
        else if(s8 == vsShowButtons[i])
        {
            ui->actionOtus->setDisabled(false);
        }
        else if(s9 == vsShowButtons[i])
        {
            ui->actionAdaptive->setDisabled(false);
        }
        else if(s10 == vsShowButtons[i])
        {
            ui->actionEqualization->setDisabled(false);
        }
        else if(s11 == vsShowButtons[i])
        {
            ui->menuConvolve_Kernel->setDisabled(false);
        }
    }

    for(int i = 0; i < vsCloseButtons.size(); i++)
    {
        if(s1 == vsCloseButtons[i])
        {
            ui->actionRGB->setDisabled(true);
        }
        else if(s2 == vsCloseButtons[i])
        {
            ui->actionGray->setDisabled(true);
        }
        else if(s3 == vsCloseButtons[i])
        {
            ui->actionMap->setDisabled(true);
        }
        else if(s4 == vsCloseButtons[i])
        {
            ui->actionsave->setDisabled(true);
        }
        else if(s5 == vsCloseButtons[i])
        {
            ui->actionsave_2->setDisabled(true);
        }
        else if(s6 == vsCloseButtons[i])
        {
            ui->actionBinary->setDisabled(true);
        }
        else if(s7 == vsCloseButtons[i])
        {
            ui->actionManual->setDisabled(true);
        }
        else if(s8 == vsCloseButtons[i])
        {
            ui->actionOtus->setDisabled(true);
        }
        else if(s9 == vsCloseButtons[i])
        {
            ui->actionAdaptive->setDisabled(true);
        }
        else if(s10 == vsShowButtons[i])
        {
            ui->actionEqualization->setDisabled(true);
        }
        else if(s11 == vsShowButtons[i])
        {
            ui->menuConvolve_Kernel->setDisabled(true);
        }
    }
}

void MainWindow::on_actionOpen_triggered()
{
    // 打开图像
    QString fileNameImg = QFileDialog::getOpenFileName(this, tr("Open Image"),"./",tr("Image File (*.jpg *.png *.bmp)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(fileNameImg).data();//filename.toAscii().data()
    cv::Mat image = cv::imread(name, -1);
    if(!image.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Image Data Is Null"));
        msgBox.exec();
    }
    else
    {
        m_ImgInfor.sPath = name;
        m_ImgInfor.nWidth = image.rows;
        m_ImgInfor.nHeight = image.cols;
        m_ImgInfor.sColorSpace = "RGB";
        connect(this,SIGNAL(sendImg(cv::Mat, INFOR_BASE::sImgInfor)),c,SLOT(OpenNew(cv::Mat, INFOR_BASE::sImgInfor)));
        emit sendImg(image, m_ImgInfor);
        disconnect(this,SIGNAL(sendImg(cv::Mat, INFOR_BASE::sImgInfor)),c,SLOT(OpenNew(cv::Mat, INFOR_BASE::sImgInfor)));
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    connect(this, SIGNAL(sendCloseImgWindow()), c, SLOT(CloseImgWindowSlot()));
    emit sendCloseImgWindow();
}
