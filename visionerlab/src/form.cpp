#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    m_pImgProcBaseObj = new CVLImgProcBase();
    QWidget::installEventFilter(this);
    m_parentCopy = parent;
    // 给dialog增加最大化最小化
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowMinMaxButtonsHint;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    // 设置鼠标移动相应
    this->setMouseTracking(true);
    // 传给centralwidget
    connect(this,SIGNAL(sendMouse(int, int, QColor,int)), m_parentCopy, SLOT(getMouse(int, int, QColor, int)));
    connect(this,SIGNAL(sendMouseDepth(int, int, ushort,int)), m_parentCopy, SLOT(getMouseDepth(int, int, ushort, int)));
    m_label = new QLabel;
    m_pCalibrationObj = new CCalibration();
    scrollArea = new QScrollArea(this);
    m_dScaling = 1.0;
    m_bCtrlPress = false;
    m_bModifyImg = false;
    vmStackBack.clear();
}

Form::~Form()
{

    if(nullptr == m_pCalibrationObj)
    {
        delete m_pCalibrationObj;
        m_pCalibrationObj = nullptr;
    }
    if(nullptr == m_pImgProcBaseObj)
    {
        delete m_pImgProcBaseObj;
        m_pImgProcBaseObj = nullptr;
    }

}
void Form::Resize(cv::Mat mSrc, cv::Mat &mDst, double dScale)
{
    int nScale = (int)dScale;
    mDst = cv::Mat(mSrc.rows * nScale, mSrc.cols * nScale, CV_8UC3, cv::Scalar(0, 0, 0));
    for(int nHeight = 0; nHeight < mSrc.rows; nHeight++)
    {
        for(int nWidth = 0; nWidth < mSrc.cols; nWidth++)
        {
            int x = mSrc.at<cv::Vec3b>(nHeight, nWidth)[0];
            int y = mSrc.at<cv::Vec3b>(nHeight, nWidth)[1];
            int z = mSrc.at<cv::Vec3b>(nHeight, nWidth)[2];
            int i = 0;
            int j = 0;
            for(int i = 0; i < dScale; i++)
            {
                for(int j = 0; j < dScale; j++)
                {
                    mDst.at<cv::Vec3b>(nHeight * nScale + i, nWidth * nScale + j)[0] = x;
                    mDst.at<cv::Vec3b>(nHeight * nScale + i, nWidth * nScale + j)[1] = y;
                    mDst.at<cv::Vec3b>(nHeight * nScale + i, nWidth * nScale + j)[2] = z;
                }
            }
        }
    }
}

void Form::Display4DepthMat(cv::Mat image, QImage &img)
{
    // BGRA转RGBA
    img = QImage(image.data,
                 image.cols,image.rows,image.step,//rgb.cols*rgb.channels()可以替换为image.step
                 QImage::Format_RGB32);
    connect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageCloseGraySlot(std::vector<std::string> ,std::vector<std::string>)));
    // 打开导航栏中的按钮
    std::vector<std::string> vsShowButtons;
    std::vector<std::string> vsCloseButtons;
    std::string s1 = "actionManual";
    std::string s2 = "actionOtus";
    std::string s3 = "actionAdaptive";
    std::string s4 = "actionConnected_Region";
    std::string s5 = "actionHit_Miss";
    std::string s6 = "actionTop_Hat";
    std::string s7 = "actionBlack_Hat";
    vsCloseButtons.push_back(s1);
    vsCloseButtons.push_back(s2);
    vsCloseButtons.push_back(s3);
    vsCloseButtons.push_back(s4);
    vsCloseButtons.push_back(s5);
    vsCloseButtons.push_back(s6);
    vsCloseButtons.push_back(s7);
    emit sendButtonShowManage(vsShowButtons, vsCloseButtons);
    disconnect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageCloseGraySlot(std::vector<std::string> ,std::vector<std::string>)));;
}

void Form::Display3DepthMat(cv::Mat image, QImage &img)
{
    // BGR转RGB
    img = QImage((const unsigned char*)(image.data),
                 image.cols,image.rows,image.cols*image.channels(),//rgb.cols*rgb.channels()可以替换为image.step
                 QImage::Format_RGB888);
    connect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageCloseGraySlot(std::vector<std::string> ,std::vector<std::string>)));
    // 打开导航栏中的按钮
    std::vector<std::string> vsShowButtons;
    std::vector<std::string> vsCloseButtons;
    std::string s1 = "actionManual";
    std::string s2 = "actionOtus";
    std::string s3 = "actionAdaptive";
    std::string s4 = "actionConnected_Region";
    std::string s5 = "actionHit_Miss";
    std::string s6 = "actionTop_Hat";
    std::string s7 = "actionBlack_Hat";
    vsCloseButtons.push_back(s1);
    vsCloseButtons.push_back(s2);
    vsCloseButtons.push_back(s3);
    vsCloseButtons.push_back(s4);
    vsCloseButtons.push_back(s5);
    vsCloseButtons.push_back(s6);
    vsCloseButtons.push_back(s7);
    emit sendButtonShowManage(vsShowButtons, vsCloseButtons);

    disconnect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageCloseGraySlot(std::vector<std::string> ,std::vector<std::string>)));
}

void Form::Display2DepthMat(cv::Mat image, QImage &img)
{
    /*
    cv::Mat imageCopy(image.rows, image.cols, CV_8UC1, cv::Scalar(0));
    double minVal = 0.0;
    double maxVal = 0.0;
    cv::minMaxLoc(image, &minVal, &maxVal);
    for(int nHeight = 0; nHeight < image.rows; nHeight++)
    {
        for(int nWidth = 0; nWidth < image.cols; nWidth++)
        {
            imageCopy.ptr<uchar>(nHeight)[nWidth] = (uchar)(((double)(image.ptr<ushort>(nHeight)[nWidth])-minVal) / maxVal * 255.0);
        }
    }

    cv::imwrite("D:\\QTProj\\ImLab\\de.png", imageCopy);
*/
    // 灰度图
    img = QImage((const unsigned char*)(image.data),
                 image.cols,image.rows,image.cols*image.channels(),
                 QImage::Format_Grayscale8);

    connect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageOpenGraySlot(std::vector<std::string> ,std::vector<std::string>)));
    // 打开导航栏中的按钮
    std::vector<std::string> vsShowButtons;
    std::vector<std::string> vsCloseButtons;
    std::string s1 = "actionEqualization";
    std::string s2 = "actionHit_Miss";
    std::string s3 = "actionTop_Hat";
    std::string s4 = "actionBlack_Hat";
    vsCloseButtons.push_back(s1);
    vsCloseButtons.push_back(s2);
    vsCloseButtons.push_back(s3);
    vsCloseButtons.push_back(s4);
    emit sendButtonShowManage(vsShowButtons, vsCloseButtons);
    disconnect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageOpenGraySlot(std::vector<std::string> ,std::vector<std::string>)));
}

void Form::Display1DepthMat(cv::Mat image, QImage &img)
{
    // 灰度图
    img = QImage((const unsigned char*)(image.data),
                 image.cols,image.rows,image.cols*image.channels(),
                 QImage::Format_Grayscale8);
    connect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageOpenGraySlot(std::vector<std::string> ,std::vector<std::string>)));
    // 打开导航栏中的按钮
    std::vector<std::string> vsShowButtons;
    std::vector<std::string> vsCloseButtons;

    std::string s5 = "actionConnected_Region";
    if(!m_pImgProcBaseObj->IfBinaryImg(image))
    {
        vsCloseButtons.push_back(s5);
    }
    else
    {
        vsShowButtons.push_back(s5);
    }
    emit sendButtonShowManage(vsShowButtons, vsCloseButtons);
    disconnect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageOpenGraySlot(std::vector<std::string> ,std::vector<std::string>)));
}

// 在显示图像这里向主界面发送信号，用以控制按钮亮灭
void Form::DisplayMat(cv::Mat mimage, QLabel *label, double dScaling)
{


    // 缩放像素级别,这里的放大效果是像素级别，如果放大2倍，某一个像素现在变为4个

    //if(dScaling > 1)
    //{
    // 转换思路，直接全部图像放大会造成计算机内存占用过大，解决方法是对显示的图像进行局部放大并显示
        //Resize(mimage, image, dScaling);
    //}
    //else
    //{

    cv::Mat image;
    if(mimage.depth() == 2)
    {
        cv::Mat imageCopy(mimage.rows, mimage.cols, CV_8UC1, cv::Scalar(0));
        double minVal = 0.0;
        double maxVal = 0.0;
        cv::minMaxLoc(mimage, &minVal, &maxVal);
        for(int nHeight = 0; nHeight < mimage.rows; nHeight++)
        {
            for(int nWidth = 0; nWidth < mimage.cols; nWidth++)
            {
                ushort us = mimage.ptr<ushort>(nHeight)[nWidth];
                uchar u = (uchar)(((double)(us)-minVal) / maxVal * 255.0);
                imageCopy.ptr<uchar>(nHeight)[nWidth] = u;
            }
        }
        cv::resize(imageCopy,image,cv::Size(0,0),dScaling,dScaling,cv::INTER_AREA);
    }
    else
    {
        cv::resize(mimage,image,cv::Size(0,0),dScaling,dScaling,cv::INTER_AREA);
    }
    QImage img;
    // Mat格式转QImage
    if(4 == mimage.channels())
    {
        Display4DepthMat(image, img);
    }
    else if(3 == mimage.channels())
    {

        Display3DepthMat(image, img);
    }
    else if(2 == mimage.depth())
    {
        Display2DepthMat(image, img);
    }
    else if(1 == mimage.channels())
    {
        Display1DepthMat(image, img);
    }
    // 需要使用copy()
    QPixmap fitpixmap = QPixmap::fromImage(img.copy());

    label->setPixmap(fitpixmap);
    // QLabel居中
    label->setAlignment(Qt::AlignCenter);
}

// 显示图像窗口
void Form::getImgCenter(cv::Mat mImg, INFOR_BASE::sImgInfor imgInfor, int nId)
{
    m_nId = nId;
    m_mImg = mImg.clone();
    m_imgInfor = imgInfor;
    /*layout布局*/

    pv = new QVBoxLayout;

    // 设置滚动条相关
    scrollArea->setWidget(m_label);
    scrollArea->setGeometry(0,0,m_label->width()+100,m_label->height()-60);
    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setAlignment(Qt::AlignCenter);
    //设置是否自动调整部件的大小.默认是false.
    scrollArea->setWidgetResizable(true);
    // 设置qlabel相关
    m_label->setStyleSheet("border:1px solid black;");
    // 设置鼠标移动相应
    vmStackBack.push_back(m_mImg.clone());

    DisplayMat(m_mImg, m_label, m_dScaling);
    // 这两个控件都要相应鼠标事件
    m_label->setMouseTracking(true);
    scrollArea->setMouseTracking(true);
    pv->addWidget(scrollArea);
    //pv->addLayout(pg,1);
    this->setLayout(pv);
    this->resize(m_mImg.cols,m_mImg.rows);
    this->show();
}
//鼠标 点击
void Form::mousePressEvent(QMouseEvent * e)
{
    //获取点击的下标
    int nMouseX = e->x();
    int nMouseY = e->y();
    QPoint  qGloalPos =  m_label->mapToGlobal(m_label->pos());
    QPoint qRelativePos;
    int x = qGloalPos.x() - m_parentCopy->geometry().x();
    int y = qGloalPos.y() - m_parentCopy->geometry().y();
    // label与图像的偏移量
    int xoffset = (m_label->contentsRect().width()-m_label->pixmap()->rect().width())/2;
    int yoffset = (m_label->contentsRect().height()-m_label->pixmap()->rect().height())/2;

    QPoint ptLabel = m_label->mapFromGlobal(QCursor::pos());
    QPoint ptPix(ptLabel.x() + xoffset, ptLabel.y() + yoffset);
    // 判断鼠标是否在图像内
    if(ptLabel.x() > xoffset && ptLabel.y() > yoffset && (ptLabel.x() < xoffset + m_label->pixmap()->rect().width()) && (ptLabel.y() < yoffset + m_label->pixmap()->rect().height()))
    {
        if(e->button() == Qt::LeftButton)
        {
            QImage image = m_label->pixmap()->toImage();
            QColor color;
            if (!image.isNull()) //如果image不为空
            {
                if (image.valid(0, 0)) //坐标位置有效
                {
                    color = image.pixel(ptLabel.x() - xoffset, ptLabel.y() - yoffset);
                }
            }
            // 槽函数应放在构造函数，虽然不知道为什么，如果放在此处，槽函数会被执行多次
            //QObject::connect(this,SIGNAL(sendMouse(int, int, QColor)), m_parentCopy, SLOT(getMouse(int, int, QColor)));

            if(2 == m_mImg.depth())
            {
                int nChannel = m_mImg.depth();
                int nX = (int)((ptLabel.x() - xoffset) / m_dScaling);
                int nY = (int)((ptLabel.y() - yoffset-1) / m_dScaling);          
                ushort uPixel = m_mImg.ptr<ushort>(nY)[nX];                
                //ushort uPixel = m_mImg.at<ushort>(nY, nX);
                emit sendMouseDepth((ptLabel.x() - xoffset) / m_dScaling, (ptLabel.y() - yoffset-1) / m_dScaling, uPixel, nChannel);
            }
            else
            {
                int nChannel = m_mImg.channels();
                emit sendMouse((ptLabel.x() - xoffset) / m_dScaling, (ptLabel.y() - yoffset-1) / m_dScaling, color, nChannel);
            }

        }
        else if(e->button() == Qt::RightButton)
        {
            qDebug() << "右键" ;
        }
    }
}

void Form::mouseMoveEvent(QMouseEvent *e)
{
    int nMouseX = e->x();
    int nMouseY = e->y();
    // label与图像的偏移量
    int xoffset = (m_label->contentsRect().width()-m_label->pixmap()->rect().width())/2;
    int yoffset = (m_label->contentsRect().height()-m_label->pixmap()->rect().height())/2;

    QPoint ptLabel = m_label->mapFromGlobal(QCursor::pos());
    QPoint ptPix(ptLabel.x() + xoffset, ptLabel.y() + yoffset);
    // 鼠标在图像区域变形状
    if(ptLabel.x() > xoffset && ptLabel.y() > yoffset && (ptLabel.x() < xoffset + m_label->pixmap()->rect().width()) && (ptLabel.y() < yoffset + m_label->pixmap()->rect().height()))
    {
        setCursor(Qt::CrossCursor);
    }
    else
    {
        setCursor(Qt::ArrowCursor);
    }
}

void Form::keyPressEvent(QKeyEvent *event)
{
    if(event->modifiers() == Qt::ControlModifier)
    {
       m_bCtrlPress = true;
       scrollArea->setAttribute(Qt::WA_TransparentForMouseEvents, true);
    }
    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_Z))
    {
        if(vmStackBack.size() > 1)
        {
            vmStackFront.push_back(vmStackBack[vmStackBack.size()-1].clone());
            vmStackBack.erase(vmStackBack.begin()+vmStackBack.size()-1);
            m_mImg = vmStackBack[vmStackBack.size() - 1].clone();
            DisplayMat(m_mImg, m_label, m_dScaling);

        }
    }

    if ((event->modifiers() == Qt::ControlModifier) && (event->key() == Qt::Key_A))
    {
        if(0 != vmStackFront.size())
        {
            vmStackBack.push_back(vmStackFront[vmStackFront.size()-1].clone());
            vmStackFront.erase(vmStackFront.begin()+vmStackFront.size()-1);
            m_mImg = vmStackBack[vmStackBack.size() - 1].clone();
            DisplayMat(m_mImg, m_label, m_dScaling);
        }
    }

}

void Form::keyReleaseEvent(QKeyEvent *ev)
{
    if(m_bCtrlPress == true)
    {
        m_bCtrlPress = false;
        scrollArea->setAttribute(Qt::WA_TransparentForMouseEvents, false);
    }
}

void Form::wheelEvent(QWheelEvent *event)
{
    if(event->delta() > 0 && QApplication::keyboardModifiers () == Qt::ControlModifier)
    {
        if(m_dScaling < 8)
        {
            m_dScaling = m_dScaling * 2;
            DisplayMat(m_mImg, m_label, m_dScaling);
        }
    }
    else if(event->delta() < 0 && QApplication::keyboardModifiers () == Qt::ControlModifier)
    {
        if(m_dScaling >= 1.0 / 32.0)
        {
            m_dScaling = m_dScaling / 2;
            DisplayMat(m_mImg, m_label, m_dScaling);
        }
    }
}

void Form::closeEvent(QCloseEvent *event) //根据不同的需求进行添加，比如：
{
    if(!m_bModifyImg)
    {
        this->close();
        return;
    }

    QString filepath = QString::fromStdString(m_imgInfor.sPath);
    int ret = QMessageBox::warning(0,"Warning",QString("%1 has been changed!\n Do you want to save it?").arg(filepath),QMessageBox::Yes |QMessageBox::No|QMessageBox::Cancel,QMessageBox::No);
    if(ret == QMessageBox::Yes) //如果选择保存文件，则执行保存操作
    {
        std::string s = filepath.toStdString();
        cv::imwrite(s, m_mImg);
        this->close();
    }
    else if(ret == QMessageBox::Cancel)
    {
        this->close();
    }
    else
    {
        this->close();
    }
}

// 窗口是否为活动窗口
bool Form::eventFilter(QObject *watched, QEvent *event)
{
    if( watched == this )
    {
        //窗口停用，变为不活动的窗口
        if(QEvent::WindowActivate == event->type())
        {
            connect(this, SIGNAL(sendFormSelf(Form *)),m_parentCopy,SLOT(ReceiveFormSelfSlot(Form *)));
            emit sendFormSelf(this);
            return true ;
        }
        else
        {
            return false ;
        }
    }
    return false ;
}
void Form::UpdateAllSlot()
{
    connect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageOpenGraySlot(std::vector<std::string> ,std::vector<std::string>)));
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
    emit sendButtonShowManage(vsShowButtons, vsCloseButtons);
    disconnect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageOpenGraySlot(std::vector<std::string> ,std::vector<std::string>)));
    DisplayMat(m_mImg, m_label, m_dScaling);
}
// 导航栏保存关闭操作
void Form::closeEventSlot()
{
    if(!m_bModifyImg)
    {

        this->close();
    }
    QString filepath = QString::fromStdString(m_imgInfor.sPath);

    int ret = QMessageBox::warning(0,"Warning",QString("%1 has been changed!\n Do you want to save it?").arg(filepath),QMessageBox::Yes |QMessageBox::No|QMessageBox::Cancel,QMessageBox::No);
    if(ret == QMessageBox::Yes) //如果选择保存文件，则执行保存操作
    {
        std::string s = filepath.toStdString();
        cv::imwrite(s, m_mImg);
        this->close();
    }
    else if(ret == QMessageBox::Cancel)
    {
        this->close();
    }
    else
    {
        this->close();
    }
}

void Form::closeEventSaveAsSlot()
{
    doFileSave();
}



void Form::doFileSave()
{
    QString dirPath = QFileDialog::getSaveFileName(NULL, QStringLiteral("ImageSaveAs"), "good.png", QString(tr("Images (*.png *.bmp *.jpg  *.gif *.jpeg)")),
                                                       Q_NULLPTR, QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    //如果选择的路径无效，则不保存
    if (!dirPath.isEmpty())
    {
        QFileInfo fileInfo(dirPath);
        if (fileInfo.exists())
            QFile::remove(dirPath);
        std::string sSaveAsDir = dirPath.toStdString();
        cv::imwrite(sSaveAsDir, m_mImg);
    }
    else
    {
        qDebug() << "empty";
    }
}

void Form::RGB2Gray()
{
    m_bModifyImg = true;
    if(3 == m_mImg.channels())
    {
        cv::cvtColor(m_mImg,m_mImg,CV_RGB2GRAY);
    }
    else if(4 == m_mImg.channels())
    {
        cv::cvtColor(m_mImg,m_mImg,CV_RGBA2GRAY);
    }
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::Gray2RGB()
{
    m_bModifyImg = true;
    if(1 == m_mImg.channels())
    {
        cv::cvtColor(m_mImg,m_mImg,CV_GRAY2RGB);
    }
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::ManualThresholdSlot()
{
    m_bModifyImg = true;
    int nDepth = 0;
    if(2 == m_mImg.depth())
    {
        nDepth = 2;
    }
    else if(1 == m_mImg.channels())
    {
        nDepth = 1;
    }
    m_dialogSlider = new DialogSlider(this, nDepth);
    m_dialogSlider->setModal(true);
    m_dialogSlider->show();
}
void Form::ManualThresholdChangeSlot(int nValue)
{
    m_bModifyImg = true;
    cv::threshold(m_mImg, m_mTem, nValue, 255, cv::THRESH_BINARY_INV);

    DisplayMat(m_mTem, m_label, m_dScaling);
}

void Form::ThresholdOtusSlot()
{
    m_bModifyImg = true;
    cv::threshold(m_mImg, m_mImg, 0, 255, CV_THRESH_OTSU);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::ThresholdAdaptiveSlot()
{
    m_bModifyImg = true;
    m_pAdaptiveDialog = new AdaptiveDialog(this);
    m_pAdaptiveDialog->setAttribute(Qt::WA_DeleteOnClose);
    m_pAdaptiveDialog->setModal(true);
    m_pAdaptiveDialog->show();
}

void Form::ThresholdAdaptiveChangeSlot(double dMaxValue, int nAdaptiveMethod, int nThresholdType, int nBlockSize, double dC)
{
    m_bModifyImg = true;
    m_mImg = m_mTem.clone();
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
    m_pAdaptiveDialog->close();
}
void Form::ThresholdAdaptiveChangeCancelSlot(double dMaxValue, int nAdaptiveMethod, int nThresholdType, int nBlockSize, double dC)
{
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::PreviewThresholdAdaptiveChangeSlot(double dMaxValue, int nAdaptiveMethod, int nThresholdType, int nBlockSize, double dC)
{
    cv::adaptiveThreshold(m_mImg, m_mTem, dMaxValue, nAdaptiveMethod, nThresholdType, nBlockSize, dC);
    DisplayMat(m_mTem, m_label, m_dScaling);
}

void Form::EqualizationSlot()
{
    m_bModifyImg = true;
    cv::equalizeHist(m_mImg, m_mImg);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::SobelSlot()
{
    m_bModifyImg = true;
    // 参数4 = 1，参数5 = 0，表示x方向的导数，垂直方向边缘
    cv::Sobel(m_mImg, m_mImg, -1, 1, 0, 3);
    // 参数4 = 0，参数5 = 1，表示y方向的导数，水平方向边缘
    // 参数6为卷积核大小，当为1时，表示1*3或3*1
    cv::Sobel(m_mImg, m_mImg, -1, 0, 1, 3);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::Scharr_3_3Slot()
{
    m_bModifyImg = true;
    // 参数4 = 1，参数5 = 0，表示x方向的导数，垂直方向边缘
    cv::Scharr(m_mImg, m_mImg, -1, 1, 0, 3);
    // 参数4 = 0，参数5 = 1，表示y方向的导数，水平方向边缘
    // 参数6为卷积核大小，当为1时，表示1*3或3*1
    cv::Scharr(m_mImg, m_mImg, -1, 0, 1, 3);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::Laplacian_5_5Slot()
{
    m_bModifyImg = true;
    cv::Laplacian(m_mImg, m_mImg, -1, 5, 1, 0, cv::BORDER_DEFAULT);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::Laplacian_7_7Slot()
{
    m_bModifyImg = true;
    cv::Laplacian(m_mImg, m_mImg, -1, 7, 1, 0, cv::BORDER_DEFAULT);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::Gaussian_3_3Slot()
{
    m_bModifyImg = true;
    cv::GaussianBlur(m_mImg, m_mImg, cv::Size(3, 3), 3, 3);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::Gaussian_5_5Slot()
{
    m_bModifyImg = true;
    cv::GaussianBlur(m_mImg, m_mImg, cv::Size(5, 5), 3, 3);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::Gaussian_7_7Slot()
{
    m_bModifyImg = true;
    cv::GaussianBlur(m_mImg, m_mImg, cv::Size(7, 7), 3, 3);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::Mean_3_3Slot()
{
    m_bModifyImg = true;
    blur(m_mImg,m_mImg, cv::Size(3,3));
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::Mean_5_5Slot()
{
    m_bModifyImg = true;
    blur(m_mImg,m_mImg, cv::Size(5,5));
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::Mean_7_7Slot()
{
    m_bModifyImg = true;
    blur(m_mImg,m_mImg, cv::Size(7,7));
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::Median_3_3Slot()
{
    m_bModifyImg = true;
    medianBlur(m_mImg,m_mImg, 3);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::Median_5_5Slot()
{
    m_bModifyImg = true;
    medianBlur(m_mImg,m_mImg, 5);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::Median_7_7Slot()
{
    m_bModifyImg = true;
    medianBlur(m_mImg,m_mImg, 7);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::ManualGaussianSlot()
{
    m_bModifyImg = true;
    m_pGaussianDialog = new GaussianDialog(this);
    m_pGaussianDialog->setAttribute(Qt::WA_DeleteOnClose);
    m_pGaussianDialog->setModal(true);
    m_pGaussianDialog->show();
}
void Form::ManualGaussianChangeSlot(int nValue)
{
    if(nValue % 2 != 1)
    {
        nValue += 1;
    }
    m_bModifyImg = true;
    cv::GaussianBlur(m_mImg, m_mTem, cv::Size(nValue, nValue), 3, 3);
    DisplayMat(m_mTem, m_label, m_dScaling);
}

void Form::ManualLaplaceSlot()
{
    m_bModifyImg = true;
    m_pLaplaceDialog = new Laplace(this);
    m_pLaplaceDialog->setAttribute(Qt::WA_DeleteOnClose);
    m_pLaplaceDialog->setModal(true);
    m_pLaplaceDialog->show();
}
void Form::ManualLaplaceChangeSlot(int nValue)
{
    if(nValue % 2 != 1)
    {
        nValue += 1;
    }
    m_bModifyImg = true;
    cv::Laplacian(m_mImg, m_mTem, -1, nValue, 1, 0, cv::BORDER_DEFAULT);
    DisplayMat(m_mTem, m_label, m_dScaling);
}

void Form::ManualErodeSlot()
{
    m_bModifyImg = true;
    m_pErodeDialog = new Erode(this);
     m_pErodeDialog->setAttribute(Qt::WA_DeleteOnClose);
    m_pErodeDialog->setModal(true);
    m_pErodeDialog->show();
}
void Form::ManualErodeChangeSlot(int nKernelSize, QString sShape)
{
    if(nKernelSize % 2 != 1)
    {
        nKernelSize += 1;
    }
    m_bModifyImg = true;
    cv::Mat erodeStruct;
    if("RECT" == sShape)
    {
        erodeStruct = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(nKernelSize,nKernelSize));
    }
    else if("CROSS" == sShape)
    {
        erodeStruct = cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(nKernelSize,nKernelSize));
    }
    else if("ELLIPSE" == sShape)
    {
        erodeStruct = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(nKernelSize,nKernelSize));
    }
    cv::erode(m_mImg, m_mTem, erodeStruct);
    DisplayMat(m_mTem, m_label, m_dScaling);
}

void Form::ManualDilateSlot()
{
    m_bModifyImg = true;
    m_pDilateDialog = new Dilate(this);
    m_pDilateDialog->setAttribute(Qt::WA_DeleteOnClose);
    m_pDilateDialog->setModal(true);
    m_pDilateDialog->show();
}
void Form::ManualDilateChangeSlot(int nKernelSize, QString sShape)
{
    if(nKernelSize % 2 != 1)
    {
        nKernelSize += 1;
    }
    m_bModifyImg = true;
    cv::Mat dilateStruct;
    if("RECT" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(nKernelSize,nKernelSize));
    }
    else if("CROSS" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(nKernelSize,nKernelSize));
    }
    else if("ELLIPSE" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(nKernelSize,nKernelSize));
    }

    cv::dilate(m_mImg, m_mTem, dilateStruct);
    DisplayMat(m_mTem, m_label, m_dScaling);
}

void Form::ManualOpenSlot()
{
    m_bModifyImg = true;
    m_pOpenDialog = new Open(this);
    m_pOpenDialog->setAttribute(Qt::WA_DeleteOnClose);
    m_pOpenDialog->setModal(true);
    m_pOpenDialog->show();
}
void Form::ManualOpenChangeSlot(int nKernelSize, QString sShape)
{
    if(nKernelSize % 2 != 1)
    {
        nKernelSize += 1;
    }
    m_bModifyImg = true;
    cv::Mat dilateStruct;
    if("RECT" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(nKernelSize,nKernelSize));
    }
    else if("CROSS" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(nKernelSize,nKernelSize));
    }
    else if("ELLIPSE" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(nKernelSize,nKernelSize));
    }
    cv::erode(m_mImg, m_mTem, dilateStruct);
    cv::dilate(m_mTem, m_mTem, dilateStruct);
    DisplayMat(m_mTem, m_label, m_dScaling);  
}

void Form::ManualCloseSlot()
{
    m_bModifyImg = true;
    m_pCloseDialog = new Close(this);
    m_pCloseDialog->setAttribute(Qt::WA_DeleteOnClose);
    m_pCloseDialog->setModal(true);
    m_pCloseDialog->show();
}
void Form::ManualCloseChangeSlot(int nKernelSize, QString sShape)
{
    if(nKernelSize % 2 != 1)
    {
        nKernelSize += 1;
    }
    m_bModifyImg = true;
    cv::Mat dilateStruct;
    if("RECT" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(nKernelSize,nKernelSize));
    }
    else if("CROSS" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(nKernelSize,nKernelSize));
    }
    else if("ELLIPSE" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(nKernelSize,nKernelSize));
    }
    cv::dilate(m_mImg, m_mTem, dilateStruct);
    cv::erode(m_mTem, m_mTem, dilateStruct);
    DisplayMat(m_mTem, m_label, m_dScaling);
}

void Form::ManualHit_MissSlot()
{
    m_bModifyImg = true;
    m_pHitMissDialog = new HitMiss(this);
    m_pHitMissDialog->setAttribute(Qt::WA_DeleteOnClose);
    m_pHitMissDialog->setModal(true);
    m_pHitMissDialog->show();
}
void Form::ManualHitMissChangeSlot(int nKernelSize, QString sShape)
{
    if(nKernelSize % 2 != 1)
    {
        nKernelSize += 1;
    }
    m_bModifyImg = true;
    cv::Mat dilateStruct;
    if("RECT" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(nKernelSize,nKernelSize));
    }
    else if("CROSS" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(nKernelSize,nKernelSize));
    }
    else if("ELLIPSE" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(nKernelSize,nKernelSize));
    }

    cv::morphologyEx(m_mImg, m_mTem, cv::MORPH_HITMISS, dilateStruct);

    DisplayMat(m_mTem, m_label, m_dScaling);
}

void Form::ManualTop_HatSlot()
{
    m_bModifyImg = true;
    m_pTopHatDialog = new TopHat(this);
    m_pTopHatDialog->setAttribute(Qt::WA_DeleteOnClose);
    m_pTopHatDialog->setModal(true);
    m_pTopHatDialog->show();
}
void Form::ManualTopHatChangeSlot(int nKernelSize, QString sShape)
{
    if(nKernelSize % 2 != 1)
    {
        nKernelSize += 1;
    }
    m_bModifyImg = true;
    cv::Mat dilateStruct;
    if("RECT" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(nKernelSize,nKernelSize));
    }
    else if("CROSS" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(nKernelSize,nKernelSize));
    }
    else if("ELLIPSE" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(nKernelSize,nKernelSize));
    }
    cv::morphologyEx(m_mImg, m_mTem, cv::MORPH_TOPHAT, dilateStruct);
    DisplayMat(m_mTem, m_label, m_dScaling);
}

void Form::ManualBlack_HatSlot()
{
    m_bModifyImg = true;
    m_pBlackHatDialog = new BlackHat(this);
    m_pBlackHatDialog->setAttribute(Qt::WA_DeleteOnClose);
    m_pBlackHatDialog->setModal(true);
    m_pBlackHatDialog->show();
}
void Form::ManualBlackHatChangeSlot(int nKernelSize, QString sShape)
{
    if(nKernelSize % 2 != 1)
    {
        nKernelSize += 1;
    }
    m_bModifyImg = true;
    cv::Mat dilateStruct;
    if("RECT" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_RECT,cv::Size(nKernelSize,nKernelSize));
    }
    else if("CROSS" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_CROSS,cv::Size(nKernelSize,nKernelSize));
    }
    else if("ELLIPSE" == sShape)
    {
        dilateStruct = cv::getStructuringElement(cv::MORPH_ELLIPSE,cv::Size(nKernelSize,nKernelSize));
    }
    cv::morphologyEx(m_mImg, m_mTem, cv::MORPH_BLACKHAT, dilateStruct);
    DisplayMat(m_mTem, m_label, m_dScaling);
}

void Form::Connected_RegionSlot()
{
    m_bModifyImg = true;
    cv::Mat mLabel1, mStats1, mCentroids1;
    int nRegionMax = 0;
    int nRegion8 = cv::connectedComponentsWithStats(m_mImg, mLabel1, mStats1, mCentroids1, 8);

    for(int i = 1; i < nRegion8; i++)
    {
        int nArea = mStats1.at<int>(i, cv::CC_STAT_AREA);
        if( nArea > nRegionMax)
        {
            nRegionMax = nArea;
        }
    }
    cv::Mat mLabel2, mStats2, mCentroids2;
    int nRegion4 = cv::connectedComponentsWithStats(m_mImg, mLabel2, mStats2, mCentroids2, 4);
    for(int i = 1; i < nRegion4; i++)
    {
        int nArea = mStats2.at<int>(i, cv::CC_STAT_AREA);
        if( nArea > nRegionMax)
        {
            nRegionMax = nArea;
        }
    }
    nRegionMax = nRegionMax * m_dScaling;

    m_pConnectedRegionDialog = new ConnectedRegion(this, nRegionMax);
    m_pConnectedRegionDialog->setAttribute(Qt::WA_DeleteOnClose);
    m_pConnectedRegionDialog->setModal(true);
    m_pConnectedRegionDialog->show();
}
void Form::ManualConnected_RegionSlot(int nValue, int ConnectedRegion48)
{
    if(ConnectedRegion48 != 4 || ConnectedRegion48 != 8)
    {
        ConnectedRegion48 = 4;
    }

    m_bModifyImg = true;
    cv::Mat mLabel, mStats, mCentroids;
    int nRegion = cv::connectedComponentsWithStats(m_mImg, mLabel, mStats, mCentroids, ConnectedRegion48);

    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    std::vector<cv::Vec3b> v3bColors;

    for(int i = 0; i < nRegion; i++)
    {
        cv::Vec3b vColor;
        if(0 == i || nValue > mStats.at<int>(i, cv::CC_STAT_AREA) )
        {
            vColor = cv::Vec3b(0,0,0);
        }
        else
        {
            vColor = cv::Vec3b(rand()%256, rand()%256, rand()%256);
        }
        v3bColors.push_back(vColor);
    }

    cv::Mat mTem = cv::Mat::zeros(m_mImg.size(), CV_8UC3);
    m_mTem = cv::Mat::zeros(m_mImg.size(), CV_8UC1);
    for(int nHeight = 0; nHeight < mTem.rows; nHeight++)
    {
        for(int nWidth = 0; nWidth < mTem.cols; nWidth++)
        {
             int nLabel = mLabel.at<int>(nHeight, nWidth);
             mTem.at<cv::Vec3b>(nHeight, nWidth) = v3bColors[nLabel];
             if(cv::Vec3b(0,0,0) != v3bColors[nLabel])
             {
                 m_mTem.at<uchar>(nHeight, nWidth) = 255;
             }

        }
    }

    DisplayMat(mTem, m_label, m_dScaling);
}

void Form::OKThresholdSliderSelectImg()
{
    m_bModifyImg = true;
    m_mImg = m_mTem.clone();
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
    m_dialogSlider->close();
}
void Form::CancelThresholdSliderSelectImg()
{
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::OKGaussianSliderSelectImg()
{
    m_bModifyImg = true;
    m_mImg = m_mTem.clone();
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
    m_pGaussianDialog->close();
}
void Form::CancelGaussianSliderSelectImg()
{
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::OKLaplaceSliderSelectImg()
{
    m_bModifyImg = true;
    m_mImg = m_mTem.clone();
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
    // m_dialogSlider->setAttribute(Qt::WA_DeleteOnClose);
    m_pLaplaceDialog->close();
}
void Form::CancelLaplaceSliderSelectImg()
{
    m_bModifyImg = true;
    DisplayMat(m_mImg, m_label, m_dScaling);
    m_pLaplaceDialog->close();
}

void Form::OKErodeSliderSelectImg()
{
    m_bModifyImg = true;
    m_mImg = m_mTem.clone();
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
    m_pErodeDialog->close();
}
void Form::CancelErodeSliderSelectImg()
{
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::OKDilateSliderSelectImg()
{
    m_bModifyImg = true;
    m_mImg = m_mTem.clone();
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
    m_pDilateDialog->close();
}
void Form::CancelDilateSliderSelectImg()
{
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::OKOpenSliderSelectImg()
{
    m_bModifyImg = true;
    m_mImg = m_mTem.clone();
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
    m_pOpenDialog->close();
}
void Form::CancelOpenSliderSelectImg()
{
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::OKCloseSliderSelectImg()
{
    m_bModifyImg = true;
    m_mImg = m_mTem.clone();
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
    m_pCloseDialog->close();
}
void Form::CancelCloseSliderSelectImg()
{
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::OKHitMissSliderSelectImg()
{
    m_bModifyImg = true;
    m_mImg = m_mTem.clone();
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
    m_pHitMissDialog->close();
}
void Form::CancelHitMissSliderSelectImg()
{
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::OKTopHatSliderSelectImg()
{
    m_bModifyImg = true;
    m_mImg = m_mTem.clone();
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
    m_pTopHatDialog->close();
}
void Form::CancelTopHatSliderSelectImg()
{
    DisplayMat(m_mImg, m_label, m_dScaling); 
}

void Form::OKBlackHatSliderSelectImg()
{
    m_bModifyImg = true;
    m_mImg = m_mTem.clone();
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
    m_pBlackHatDialog->close();
}
void Form::CancelBlackHatSliderSelectImg()
{
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::OKConnected_RegionSliderSelectImg()
{
    m_bModifyImg = true;
    m_mImg = m_mTem.clone();
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
    m_pConnectedRegionDialog->close();
}
void Form::CancelConnected_RegionSliderSelectImg()
{
    DisplayMat(m_mImg, m_label, m_dScaling);
}

// 翻转
void Form::FlipXSlot()
{
    m_bModifyImg = true;
    cv::flip(m_mImg,m_mImg, 0);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::FlipYSlot()
{
    m_bModifyImg = true;
    cv::flip(m_mImg,m_mImg, 1);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::FlipXYSlot()
{
    m_bModifyImg = true;
    cv::flip(m_mImg,m_mImg, -1);
    vmStackFront.clear();
    vmStackBack.push_back(m_mImg.clone());
    DisplayMat(m_mImg, m_label, m_dScaling);
}
