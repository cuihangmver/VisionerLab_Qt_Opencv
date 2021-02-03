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
    connect(ui->actionErode, SIGNAL(triggered()), c,SLOT(ErodeSlot()));
    connect(ui->actionDilate, SIGNAL(triggered()), c,SLOT(DilateSlot()));
    connect(ui->actionOpen_2, SIGNAL(triggered()), c,SLOT(OpenSlot()));
    connect(ui->actionClose_2, SIGNAL(triggered()), c,SLOT(CloseSlot()));
    connect(ui->actionHit_Miss, SIGNAL(triggered()), c,SLOT(Hit_MissSlot()));
    connect(ui->actionTop_Hat, SIGNAL(triggered()), c,SLOT(Top_HatSlot()));
    connect(ui->actionBlack_Hat, SIGNAL(triggered()), c,SLOT(Black_HatSlot()));
    connect(ui->actionStructured_Light, SIGNAL(triggered()), c,SLOT(Structured_LightSlot()));
    connect(ui->actionEyeHandCalibration, SIGNAL(triggered()), c,SLOT(EyeHandCalibrationSlot()));

    // 翻转
    connect(ui->actionFlip_X_2, SIGNAL(triggered()), c,SLOT(FlipXSlot()));
    connect(ui->actionFlip_Y_2, SIGNAL(triggered()), c,SLOT(FlipYSlot()));
    connect(ui->actionFlip_XY, SIGNAL(triggered()), c,SLOT(FlipXYSlot()));

    // 输出LOG信息，定义一次，全局都能用
    // qDebug：     调试信息
    // qWarning：   警告信息
    // qCritical：  严重错误
    // qFatal：     致命错误
    qInstallMessageHandler(outputMessage);
    qDebug("11111111");
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
    ui->actionGaussian->setDisabled(true);
    ui->actionLaplace->setDisabled(true);
    ui->actionConnected_Region->setDisabled(true);
    ui->actionErode->setDisabled(true);
    ui->actionDilate->setDisabled(true);
    ui->actionOpen_2->setDisabled(true);
    ui->actionClose_2->setDisabled(true);
    ui->actionHit_Miss->setDisabled(true);
    ui->actionTop_Hat->setDisabled(true);
    ui->actionBlack_Hat->setDisabled(true);
    ui->actionMean->setDisabled(true);
    ui->actionMedian->setDisabled(true);

    ui->actionFlip_X_2->setDisabled(true);
    ui->actionFlip_Y_2->setDisabled(true);
    ui->actionFlip_XY->setDisabled(true);
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
    std::string s12 = "actionGaussian";
    std::string s13 = "actionLaplace";
    std::string s14 = "actionConnected_Region";
    std::string s15 = "actionErode";
    std::string s16 = "actionDilate";
    std::string s17 = "actionOpen_2";
    std::string s18 = "actionClose_2";
    std::string s19 = "actionHit_Miss";
    std::string s20 = "actionTop_Hat";
    std::string s21 = "actionBlack_Hat";

    std::string s22 = "actionFlip_X_2";
    std::string s23 = "actionFlip_Y_2";
    std::string s24 = "actionFlip_XY";

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
        else if(s12 == vsShowButtons[i])
        {
            ui->actionGaussian->setDisabled(false);
        }
        else if(s13 == vsShowButtons[i])
        {
            ui->actionLaplace->setDisabled(false);
        }
        else if(s14 == vsShowButtons[i])
        {
            ui->actionConnected_Region->setDisabled(false);
        }
        else if(s15 == vsShowButtons[i])
        {
            ui->actionErode->setDisabled(false);
        }
        else if(s16 == vsShowButtons[i])
        {
            ui->actionDilate->setDisabled(false);
        }
        else if(s17 == vsShowButtons[i])
        {
            ui->actionOpen_2->setDisabled(false);
        }
        else if(s18 == vsShowButtons[i])
        {
            ui->actionClose_2->setDisabled(false);
        }
        else if(s19 == vsShowButtons[i])
        {
            ui->actionHit_Miss->setDisabled(false);
        }
        else if(s20 == vsShowButtons[i])
        {
            ui->actionTop_Hat->setDisabled(false);
        }
        else if(s21 == vsShowButtons[i])
        {
            ui->actionBlack_Hat->setDisabled(false);
        }
        else if(s22 == vsShowButtons[i])
        {
            ui->actionFlip_X_2->setDisabled(false);
        }
        else if(s23 == vsShowButtons[i])
        {
            ui->actionFlip_Y_2->setDisabled(false);
        }
        else if(s24 == vsShowButtons[i])
        {
            ui->actionFlip_XY->setDisabled(false);
        }
    }

    for(int i = 0; i < vsCloseButtons.size(); i++)
    {
        QString qstr2 = QString::fromStdString(vsCloseButtons[i]);

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
        else if(s10 == vsCloseButtons[i])
        {
            ui->actionEqualization->setDisabled(true);
        }
        else if(s11 == vsCloseButtons[i])
        {
            ui->menuConvolve_Kernel->setDisabled(true);
        }
        else if(s12 == vsCloseButtons[i])
        {
            ui->actionGaussian->setDisabled(true);
        }
        else if(s13 == vsCloseButtons[i])
        {
            ui->actionLaplace->setDisabled(true);
        }
        else if(s14 == vsCloseButtons[i])
        {
            ui->actionConnected_Region->setDisabled(true);
        }
        else if(s15 == vsCloseButtons[i])
        {
            ui->actionErode->setDisabled(true);
        }
        else if(s16 == vsCloseButtons[i])
        {
            ui->actionDilate->setDisabled(true);
        }
        else if(s17 == vsCloseButtons[i])
        {
            ui->actionOpen_2->setDisabled(true);
        }
        else if(s18 == vsCloseButtons[i])
        {
            ui->actionClose_2->setDisabled(true);
        }
        else if(s19 == vsCloseButtons[i])
        {
            ui->actionHit_Miss->setDisabled(true);
        }
        else if(s20 == vsCloseButtons[i])
        {
            ui->actionTop_Hat->setDisabled(true);
        }
        else if(s21 == vsCloseButtons[i])
        {
            ui->actionBlack_Hat->setDisabled(true);
        }
        else if(s22 == vsShowButtons[i])
        {
            ui->actionFlip_X_2->setDisabled(true);
        }
        else if(s23 == vsShowButtons[i])
        {
            ui->actionFlip_Y_2->setDisabled(true);
        }
        else if(s24 == vsShowButtons[i])
        {
            ui->actionFlip_XY->setDisabled(true);
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


