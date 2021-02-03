#include "centralwidget.h"
#include "ui_centralwidget.h"

CentralWidget::CentralWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CentralWidget)
{
    ui->setupUi(this);
    // 允许拖拽放入
    this->setAcceptDrops(true);
}
CentralWidget::CentralWidget(int a, QWidget *parent)
{
    vForm.clear();
    m_parentCopy = parent;
    m_textedit = new QTextEdit();
    // 对主界面进行布局

    qButtonVer = new QVBoxLayout;
    m_textedit = new QTextEdit();
    m_textedit->setReadOnly(true);
    this->setAcceptDrops(true);
    // 版本信息
    QString sInforVL = "VisionerLab 1.0 - 09 Oct 2020 \r\n";
    QString sInforOpenCv = "OpenCv 3.4.0\r\n";
    QString sInforQT = "QT 5.12.0\r\n";
    QString sInfor = sInforVL + sInforOpenCv + sInforQT;
    m_textedit->insertPlainText(sInfor);
    qButtonVer->addWidget(m_textedit);
    this->setLayout(qButtonVer);
    this->resize(500,500);
    m_nId = 0;
    this->show();
}

CentralWidget::~CentralWidget()
{

}

void CentralWidget::dragEnterEvent(QDragEnterEvent *event)
{
    if(event->mimeData()->hasUrls())
    {
        event->acceptProposedAction();
    }
    else
    {
        //event->ignore();
    }
}

void CentralWidget::dropEvent(QDropEvent *event)
{
    const QMimeData *mimeData = event->mimeData(); 
    if(!mimeData->hasUrls())
    {
        return;
    }
    QList<QUrl> urlList = mimeData->urls();

    for(int i = 0; i < urlList.size(); i++)
    {
        QString fileName = urlList.at(i).toLocalFile();

        if(fileName.isEmpty())
        {
            return;
        }
        QTextCodec *code = QTextCodec::codecForName("gb18030");
        std::string name = code->fromUnicode(fileName).data();//filename.toAscii().data()
        cv::Mat image = cv::imread(name, -1);

        if(!image.data)
        {
            QMessageBox msgBox;
            msgBox.setText(tr("Image Data Is Null"));
            msgBox.exec();
        }
        else
        {

            connect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManage(std::vector<std::string> ,std::vector<std::string>)));
            // 打开导航栏中的按钮
            std::vector<std::string> vsShowButtons;
            std::vector<std::string> vsCloseButtons;
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
            vsShowButtons.push_back(s1);
            vsShowButtons.push_back(s2);
            vsShowButtons.push_back(s3);
            vsShowButtons.push_back(s4);
            vsShowButtons.push_back(s5);
            vsShowButtons.push_back(s6);
            vsShowButtons.push_back(s7);
            vsShowButtons.push_back(s8);
            vsShowButtons.push_back(s9);
            vsShowButtons.push_back(s10);
            vsShowButtons.push_back(s11);
            vsShowButtons.push_back(s12);
            vsShowButtons.push_back(s13);
            vsShowButtons.push_back(s14);
            vsShowButtons.push_back(s15);
            vsShowButtons.push_back(s16);
            vsShowButtons.push_back(s17);
            vsShowButtons.push_back(s18);
            vsShowButtons.push_back(s19);
            vsShowButtons.push_back(s20);
            vsShowButtons.push_back(s21);
            vsShowButtons.push_back(s22);
            vsShowButtons.push_back(s23);
            vsShowButtons.push_back(s24);
            emit sendButtonShowManage(vsShowButtons, vsCloseButtons);
            disconnect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManage(std::vector<std::string> ,std::vector<std::string>)));


            // 将显示图像窗口打开
            // 关闭窗口应将内存释放！！！！
            Form *openimgMul = new Form(this);
            openimgMul->setAttribute(Qt::WA_DeleteOnClose);
            S_FORM sForm;
            sForm.id = m_nId;
            m_nId++;
            sForm.form = openimgMul;
            //vForm.push_back(sForm);
            m_openimg = openimgMul;
            connect(this, SIGNAL(sendImgCenter(cv::Mat, INFOR_BASE::sImgInfor, int)),openimgMul,SLOT(getImgCenter(cv::Mat, INFOR_BASE::sImgInfor, int)));
            cv::Mat rgb;
            if (3 == image.channels())
            {
                m_ImgInfor.sPath = name;
                m_ImgInfor.nWidth = image.rows;
                m_ImgInfor.nHeight = image.cols;
                m_ImgInfor.sColorSpace = "3RGB";
                cvtColor(image,rgb,CV_BGR2RGB);
                emit sendImgCenter(rgb, m_ImgInfor, m_nId);
            }
            else if (4 == image.channels())
            {
                m_ImgInfor.sPath = name;
                m_ImgInfor.nWidth = image.rows;
                m_ImgInfor.nHeight = image.cols;
                m_ImgInfor.sColorSpace = "4RGB";
                emit sendImgCenter(image, m_ImgInfor, m_nId);
            }
            else if (2 == image.depth())
            {
                m_ImgInfor.sPath = name;
                m_ImgInfor.nWidth = image.rows;
                m_ImgInfor.nHeight = image.cols;
                m_ImgInfor.sColorSpace = "2GRAY";
                emit sendImgCenter(image, m_ImgInfor, m_nId);
            }
            else if (1 == image.channels())
            {
                m_ImgInfor.sPath = name;
                m_ImgInfor.nWidth = image.rows;
                m_ImgInfor.nHeight = image.cols;
                m_ImgInfor.sColorSpace = "1GRAY";
                emit sendImgCenter(image, m_ImgInfor, m_nId);
            }
            disconnect(this, SIGNAL(sendImgCenter(cv::Mat, INFOR_BASE::sImgInfor, int)),openimgMul,SLOT(getImgCenter(cv::Mat, INFOR_BASE::sImgInfor, int)));
            openimgMul->show();
        }
    }

}
void CentralWidget::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}




void CentralWidget::OpenNew(cv::Mat image, INFOR_BASE::sImgInfor imginfor)
{
    m_ImgInfor.sPath = imginfor.sPath;
    m_ImgInfor.nWidth = image.rows;
    m_ImgInfor.nHeight = image.cols;
    m_ImgInfor.sColorSpace = "RGB";
    connect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManage(std::vector<std::string> ,std::vector<std::string>)));
    std::vector<std::string> vsShowButtons;
    std::vector<std::string> vsCloseButtons;
    std::string s1 = "actionRGB";
    std::string s2 = "actionGray";
    std::string s3 = "actionMap";
    std::string s4 = "actionsave";
    std::string s5 = "actionsave_2";
    std::string s6 = "actionBinary";
    std::string s7 = "actionManual";
    vsShowButtons.push_back(s1);
    vsShowButtons.push_back(s2);
    vsShowButtons.push_back(s3);
    vsShowButtons.push_back(s4);
    vsShowButtons.push_back(s5);
    vsShowButtons.push_back(s6);
    vsShowButtons.push_back(s7);
    emit sendButtonShowManage(vsShowButtons, vsCloseButtons);
    disconnect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManage(std::vector<std::string> ,std::vector<std::string>)));
    // 打开显示图像窗口
    openimg = new Form(this);
    m_openimg = openimg;
    connect(this, SIGNAL(sendImgCenter(cv::Mat, INFOR_BASE::sImgInfor)),openimg,SLOT(getImgCenter(cv::Mat, INFOR_BASE::sImgInfor)));
    cv::Mat rgb;
    if (3 == image.channels())
    {
        cvtColor(image,rgb,CV_BGR2RGB);
        emit sendImgCenter(rgb, m_ImgInfor, 1);
    }
    else
    {
        emit sendImgCenter(image, m_ImgInfor, 1);
    }
    disconnect(this, SIGNAL(sendImgCenter(cv::Mat, INFOR_BASE::sImgInfor, int)),openimg,SLOT(getImgCenter(cv::Mat, INFOR_BASE::sImgInfor, int)));
    openimg->show();
}

// 得到鼠标点击坐标及像素值并显示
void CentralWidget::getMouse(int nMouseX, int nMouseY, QColor color, int nChannel)
{
    QString sCorX = QString::number(nMouseX);
    QString sCorY = QString::number(nMouseY);
    QString sRed = QString::number(color.red());
    QString sGreen = QString::number(color.green());
    QString sBlue = QString::number(color.blue());
    QString sAlpha = QString::number(color.alpha());
    QString sShow;
    if(1 == nChannel)
    {
        sShow = "(" + sCorX + ", " + sCorY + ") = " + "[" + sRed + "]" + "\r\n";
    }
    else if(3 == nChannel)
    {
        sShow = "(" + sCorX + ", " + sCorY + ") = " + "[" + sRed + ", " + sGreen + ", " + sBlue + "]" + "\r\n";
    }
    else if(4 == nChannel)
    {
        sShow = "(" + sCorX + ", " + sCorY + ") = " + "[" + sRed + ", " + sGreen + ", " + sBlue + ", " + sAlpha + "]" + "\r\n";
    }

    // 插入textedit显示
    m_textedit->insertPlainText(sShow);
    m_textedit->moveCursor(QTextCursor::End);
}

void CentralWidget::getMouseDepth(int nMouseX, int nMouseY, ushort uPixel, int nChannel)
{
    QString sCorX = QString::number(nMouseX);
    QString sCorY = QString::number(nMouseY);
    QString sPixel = QString::number(uPixel);
    QString sShow;
    sShow = "(" + sCorX + ", " + sCorY + ") = " + "[" + sPixel + "]" + "\r\n";

    // 插入textedit显示
    m_textedit->insertPlainText(sShow);
    m_textedit->moveCursor(QTextCursor::End);
}

void CentralWidget::RGB2Gray()
{
    connect(this, SIGNAL(sendRGB2Gray()), m_openimg, SLOT(RGB2Gray()));
    emit sendRGB2Gray();
}
void CentralWidget::Gray2RGB()
{
    connect(this, SIGNAL(sendGray2RGB()), m_openimg, SLOT(Gray2RGB()));
    emit sendGray2RGB();
}
void CentralWidget::closeEventSlot()
{
    connect(this, SIGNAL(sendcloseEventSlot()), m_openimg, SLOT(closeEventSlot()));
    emit sendcloseEventSlot();
}
void CentralWidget::closeEventSaveAsSlot()
{
    connect(this, SIGNAL(sendcloseEventSaveAsSlot()), m_openimg, SLOT(closeEventSaveAsSlot()));
    emit sendcloseEventSaveAsSlot();
}

void CentralWidget::ManualThresholdSlot()
{
    connect(this, SIGNAL(sendManualThresholdSlot()), m_openimg, SLOT(ManualThresholdSlot()));
    emit sendManualThresholdSlot(); 
    disconnect(this, SIGNAL(sendManualThresholdSlot()), m_openimg, SLOT(ManualThresholdSlot()));
}
void CentralWidget::ThresholdOtusSlot()
{
    connect(this, SIGNAL(sendThresholdOtusSlot()), m_openimg, SLOT(ThresholdOtusSlot()));
    emit sendThresholdOtusSlot();
}
void CentralWidget::ThresholdAdaptiveSlot()
{
    connect(this, SIGNAL(sendThresholdAdaptiveSlot()), m_openimg, SLOT(ThresholdAdaptiveSlot()));
    emit sendThresholdAdaptiveSlot();
    disconnect(this, SIGNAL(sendThresholdAdaptiveSlot()), m_openimg, SLOT(ThresholdAdaptiveSlot()));
}

void CentralWidget::EqualizationSlot()
{
    connect(this, SIGNAL(sendEqualizationSlot()), m_openimg, SLOT(EqualizationSlot()));
    emit sendEqualizationSlot();
}

void CentralWidget::SobelSlot()
{
    connect(this, SIGNAL(sendSobelSlot()), m_openimg, SLOT(SobelSlot()));
    emit sendSobelSlot();
}

void CentralWidget::Scharr_3_3Slot()
{
    connect(this, SIGNAL(sendScharr_3_3Slot()), m_openimg, SLOT(Scharr_3_3Slot()));
    emit sendScharr_3_3Slot();
}

void CentralWidget::Laplacian_5_5Slot()
{
    connect(this, SIGNAL(sendLaplacian_5_5Slot()), m_openimg, SLOT(Laplacian_5_5Slot()));
    emit sendLaplacian_5_5Slot();
}
void CentralWidget::Laplacian_7_7Slot()
{
    connect(this, SIGNAL(sendLaplacian_7_7Slot()), m_openimg, SLOT(Laplacian_7_7Slot()));
    emit sendLaplacian_7_7Slot();
}
void CentralWidget::Gaussian_3_3Slot()
{
    connect(this, SIGNAL(sendGaussian_3_3Slot()), m_openimg, SLOT(Gaussian_3_3Slot()));
    emit sendGaussian_3_3Slot();
}
void CentralWidget::Gaussian_5_5Slot()
{
    connect(this, SIGNAL(sendGaussian_5_5Slot()), m_openimg, SLOT(Gaussian_5_5Slot()));
    emit sendGaussian_5_5Slot();
}
void CentralWidget::Gaussian_7_7Slot()
{
    connect(this, SIGNAL(sendGaussian_7_7Slot()), m_openimg, SLOT(Gaussian_7_7Slot()));
    emit sendGaussian_7_7Slot();
}
void CentralWidget::Mean_3_3Slot()
{
    connect(this, SIGNAL(sendMean_3_3Slot()), m_openimg, SLOT(Mean_3_3Slot()));
    emit sendMean_3_3Slot();
}
void CentralWidget::Mean_5_5Slot()
{
    connect(this, SIGNAL(sendMean_5_5Slot()), m_openimg, SLOT(Mean_5_5Slot()));
    emit sendMean_5_5Slot();
}
void CentralWidget::Mean_7_7Slot()
{
    connect(this, SIGNAL(sendMean_7_7Slot()), m_openimg, SLOT(Mean_7_7Slot()));
    emit sendMean_7_7Slot();
}
void CentralWidget::Median_3_3Slot()
{
    connect(this, SIGNAL(sendMedian_3_3Slot()), m_openimg, SLOT(Median_3_3Slot()));
    emit sendMedian_3_3Slot();
}
void CentralWidget::Median_5_5Slot()
{
    connect(this, SIGNAL(sendMedian_5_5Slot()), m_openimg, SLOT(Median_5_5Slot()));
    emit sendMedian_5_5Slot();
}
void CentralWidget::Median_7_7Slot()
{
    connect(this, SIGNAL(sendMedian_7_7Slot()), m_openimg, SLOT(Median_7_7Slot()));
    emit sendMedian_7_7Slot();
}
void CentralWidget::GaussianSlot()
{
    connect(this, SIGNAL(sendGaussianSlot()), m_openimg, SLOT(ManualGaussianSlot()));
    emit sendGaussianSlot();
    disconnect(this, SIGNAL(sendGaussianSlot()), m_openimg, SLOT(ManualGaussianSlot()));
}
void CentralWidget::LaplaceSlot()
{
    connect(this, SIGNAL(sendLaplaceSlot()), m_openimg, SLOT(ManualLaplaceSlot()));
    emit sendLaplaceSlot();
    disconnect(this, SIGNAL(sendLaplaceSlot()), m_openimg, SLOT(ManualLaplaceSlot()));
}
void CentralWidget::Connected_RegionSlot()
{
    connect(this, SIGNAL(sendConnected_RegionSlot()), m_openimg, SLOT(Connected_RegionSlot()));
    emit sendConnected_RegionSlot();
    disconnect(this, SIGNAL(sendConnected_RegionSlot()), m_openimg, SLOT(Connected_RegionSlot()));
}


void CentralWidget::ErodeSlot()
{
    connect(this, SIGNAL(sendErodeSlot()), m_openimg, SLOT(ManualErodeSlot()));
    emit sendErodeSlot();
    disconnect(this, SIGNAL(sendErodeSlot()), m_openimg, SLOT(ManualErodeSlot()));
}


void CentralWidget::DilateSlot()
{
    connect(this, SIGNAL(sendDilateSlot()), m_openimg, SLOT(ManualDilateSlot()));
    emit sendDilateSlot();
    disconnect(this, SIGNAL(sendDilateSlot()), m_openimg, SLOT(ManualDilateSlot()));
}

void CentralWidget::OpenSlot()
{
    connect(this, SIGNAL(sendOpenSlot()), m_openimg, SLOT(ManualOpenSlot()));
    emit sendOpenSlot();
    disconnect(this, SIGNAL(sendOpenSlot()), m_openimg, SLOT(ManualOpenSlot()));
}

void CentralWidget::CloseSlot()
{
    connect(this, SIGNAL(sendCloseSlot()), m_openimg, SLOT(ManualCloseSlot()));
    emit sendCloseSlot();
    disconnect(this, SIGNAL(sendCloseSlot()), m_openimg, SLOT(ManualCloseSlot()));
}


void CentralWidget::Hit_MissSlot()
{
    connect(this, SIGNAL(sendHit_MissSlot()), m_openimg, SLOT(ManualHit_MissSlot()));
    emit sendHit_MissSlot();
    disconnect(this, SIGNAL(sendHit_MissSlot()), m_openimg, SLOT(ManualHit_MissSlot()));
}

void CentralWidget::Top_HatSlot()
{
    connect(this, SIGNAL(sendTop_HatSlot()), m_openimg, SLOT(ManualTop_HatSlot()));
    emit sendTop_HatSlot();
    disconnect(this, SIGNAL(sendTop_HatSlot()), m_openimg, SLOT(ManualTop_HatSlot()));
}

void CentralWidget::Black_HatSlot()
{
    connect(this, SIGNAL(sendBlack_HatSlot()), m_openimg, SLOT(ManualBlack_HatSlot()));
    emit sendBlack_HatSlot();
    disconnect(this, SIGNAL(sendBlack_HatSlot()), m_openimg, SLOT(ManualBlack_HatSlot()));
}


void CentralWidget::ButtonShowManageCloseGraySlot(std::vector<std::string> vsShowButtons, std::vector<std::string> vsCloseButtons)
{
    connect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManage(std::vector<std::string> ,std::vector<std::string>)));
    emit sendButtonShowManage(vsShowButtons, vsCloseButtons);
}
void CentralWidget::ButtonShowManageOpenGraySlot(std::vector<std::string> vsShowButtons, std::vector<std::string> vsCloseButtons)
{
    connect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManage(std::vector<std::string> ,std::vector<std::string>)));
    emit sendButtonShowManage(vsShowButtons, vsCloseButtons);
}

void CentralWidget::CloseImgWindowFromFormSlot(int nId)
{
    m_openimg->close(); 
}

void CentralWidget::ReceiveFormSelfSlot(Form * pFormObj)
{
    m_openimg = pFormObj;
    connect(this, SIGNAL(sendUpdateAllSlot()), m_openimg, SLOT(UpdateAllSlot()));
    emit sendUpdateAllSlot();
    disconnect(this, SIGNAL(sendUpdateAllSlot()), m_openimg, SLOT(UpdateAllSlot()));
}

void CentralWidget::Structured_LightSlot()
{
    m_pCalibrationObj = new CCalibration();
    m_pCStructLightObj = new CStructuredLightCalibration(this);
    m_pCStructLightObj->setModal(true);
    m_pCStructLightObj->setAttribute(Qt::WA_DeleteOnClose);
    m_pCStructLightObj->show();
}

void CentralWidget::Structured_LightParamSlot
(
        std::string sPath, cv::Point pWidthHeight,
        double dPerMove, double dDis2Line, double dC
)
{
    std::string s = sPath;
    QString ss = QString::fromStdString(s);
    std::vector<cv::Point> vpLeft;
    std::vector<cv::Point> vpRight;
    //cv::Point pWidthHeight;
    //pWidthHeight.x = 2592;
    //pWidthHeight.y = 1944;
    double dcot;
    double dBaseLine;
    double dF;
    double dCenterDis;
    m_pCalibrationObj->StructedLightReadData(s, vpLeft, vpRight);
    m_pCalibrationObj->StructedLightCalibration
            (vpLeft, vpRight,
             pWidthHeight, dPerMove, dDis2Line, dC,
             dCenterDis, dcot, dBaseLine, dF);
    qDebug() << "dCenterDis = " << dCenterDis;
    qDebug() << "dcot = " << dcot;
    qDebug() << "dBaseLine = " << dBaseLine;
    qDebug() << "dF = " << dF;
}

void CentralWidget::EyeHandCalibrationSlot()
{
    m_pCalibrationObj = new CCalibration();
    m_pEyeHandCalibration = new EyeHandCalibration(this);
    m_pEyeHandCalibration->setModal(true);
    m_pEyeHandCalibration->setAttribute(Qt::WA_DeleteOnClose);
    m_pEyeHandCalibration->show();
}

void CentralWidget::EyeHandCalibrationParamSlot
(
        std::string sPathTCP, std::string sPathECam
)
{
    std::vector<Eigen::MatrixXd> vmEyeHand;
    std::vector<Eigen::MatrixXd> vmTCP;
    m_pCalibrationObj->HandEyeReadData(sPathECam, sPathTCP, vmEyeHand, vmTCP);
    Eigen::Matrix4d mHandEye = m_pCalibrationObj->HandEyeCalibration(vmEyeHand, vmTCP);
    qDebug() << mHandEye(0, 0) << " " << mHandEye(0, 1)
             << " " << mHandEye(0, 2) << " " << mHandEye(0, 3);
    qDebug() << mHandEye(1, 0) << " " << mHandEye(1, 1)
             << " " << mHandEye(1, 2) << " " << mHandEye(1, 3);
    qDebug() << mHandEye(2, 0) << " " << mHandEye(2, 1)
             << " " << mHandEye(2, 2) << " " << mHandEye(2, 3);
    qDebug() << mHandEye(3, 0) << " " << mHandEye(3, 1)
             << " " << mHandEye(3, 2) << " " << mHandEye(3, 3);
    Eigen::Vector4d vPointCam(0.0, 0.0, 340.0, 1.0);

    Eigen::Vector4d vPointEnd(4, 1);
    vPointEnd = mHandEye * vPointCam;
    Eigen::Matrix4d mTCP;

    double dRX = 93.9342 / 180.0 * 3.1415926;
    double dRY = -69.7619 / 180.0 * 3.1415926;
    double dRZ = -1.1832 / 180.0 * 3.1415926;
    Eigen::Vector3d vEuler(dRX, dRY, dRZ);
    // 欧拉角转旋转矩阵
    //Eigen::Matrix3d mTCPMatrix3 = Eigen::AngleAxisd(vEuler[0], Eigen::Vector3d::UnitX()) *
    //              Eigen::AngleAxisd(vEuler[1], Eigen::Vector3d::UnitY()) *
     //             Eigen::AngleAxisd(vEuler[2], Eigen::Vector3d::UnitZ());


}

// 翻转
void CentralWidget::FlipXSlot()
{
    connect(this, SIGNAL(sendFlipXSlot()), m_openimg, SLOT(FlipXSlot()));
    emit sendFlipXSlot();
}
void CentralWidget::FlipYSlot()
{
    connect(this, SIGNAL(sendFlipYSlot()), m_openimg, SLOT(FlipYSlot()));
    emit sendFlipYSlot();
}
void CentralWidget::FlipXYSlot()
{
    connect(this, SIGNAL(sendFlipXYSlot()), m_openimg, SLOT(FlipXYSlot()));
    emit sendFlipXYSlot();
}
