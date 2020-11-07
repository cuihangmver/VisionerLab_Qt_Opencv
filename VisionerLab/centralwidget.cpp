#include "centralwidget.h"
#include "ui_centralwidget.h"

CentralWidget::CentralWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CentralWidget)
{
    ui->setupUi(this);
    this->setAcceptDrops(true);
}
CentralWidget::CentralWidget(int a, QWidget *parent)
{
    m_parentCopy = parent;
    m_textedit = new QTextEdit();
    // 对主界面进行布局
    // button的布局
    qButtonHor = new QHBoxLayout;
    qButtonVer = new QVBoxLayout;
    btn1 = new QPushButton;
    btn2 = new QPushButton;
    btn3 = new QPushButton;
    btn1 ->setText("back");
    btn2 ->setText("ahead");
    btn3 ->setText("histogram");
    qButtonHor->addWidget(btn1);
    qButtonHor->addWidget(btn2);
    qButtonHor->addWidget(btn3);
    qButtonVer->addLayout(qButtonHor,1);
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
    this->show();
}

CentralWidget::~CentralWidget()
{
    if(nullptr != btn1)
    {
        delete btn1;
        btn1 = nullptr;
    }
    if(nullptr != btn2)
    {
        delete btn2;
        btn2 = nullptr;
    }
    if(nullptr != btn3)
    {
        delete btn3;
        btn3 = nullptr;
    }
    if(nullptr != qButtonHor)
    {
        delete qButtonHor;
        qButtonHor = nullptr;
    }
    if(nullptr != qButtonVer)
    {
        delete qButtonVer;
        qButtonVer = nullptr;
    }
    if(nullptr != m_textedit)
    {
        delete m_textedit;
        m_textedit = nullptr;
    }
    if(nullptr != m_openimg)
    {
        delete m_openimg;
        m_openimg = nullptr;
    }
    if(nullptr != ui)
    {
        delete ui;
        ui = nullptr;
    }
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

    //如果同时拖入了多个资源，只选择第一个
    QString fileName = urlList.at(0).toLocalFile();
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
        // 将灰色按钮打开
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

        // 将显示图像窗口打开
        // 关闭窗口应将内存释放！！！！
        openimg = new Form(this);
        m_openimg = openimg;
        connect(this, SIGNAL(sendImgCenter(cv::Mat, INFOR_BASE::sImgInfor)),openimg,SLOT(getImgCenter(cv::Mat, INFOR_BASE::sImgInfor)));
        cv::Mat rgb;

        if (3 == image.channels())
        {
            m_ImgInfor.sPath = name;
            m_ImgInfor.nWidth = image.rows;
            m_ImgInfor.nHeight = image.cols;
            m_ImgInfor.sColorSpace = "3RGB";
            cvtColor(image,rgb,CV_BGR2RGB);
            emit sendImgCenter(rgb, m_ImgInfor);
        }
        else if (4 == image.channels())
        {
            m_ImgInfor.sPath = name;
            m_ImgInfor.nWidth = image.rows;
            m_ImgInfor.nHeight = image.cols;
            m_ImgInfor.sColorSpace = "4RGB";
            emit sendImgCenter(image, m_ImgInfor);
        }
        else if (2 == image.depth())
        {
            m_ImgInfor.sPath = name;
            m_ImgInfor.nWidth = image.rows;
            m_ImgInfor.nHeight = image.cols;
            m_ImgInfor.sColorSpace = "2GRAY";
            emit sendImgCenter(image, m_ImgInfor);
        }
        else if (1 == image.channels())
        {
            qDebug() << "image.depth = " << image.depth();
            m_ImgInfor.sPath = name;
            m_ImgInfor.nWidth = image.rows;
            m_ImgInfor.nHeight = image.cols;
            m_ImgInfor.sColorSpace = "1GRAY";
            emit sendImgCenter(image, m_ImgInfor);
        }
        disconnect(this, SIGNAL(sendImgCenter(cv::Mat, INFOR_BASE::sImgInfor)),openimg,SLOT(getImgCenter(cv::Mat, INFOR_BASE::sImgInfor)));
        openimg->show();
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
        emit sendImgCenter(rgb, m_ImgInfor);
    }
    else
    {
        emit sendImgCenter(image, m_ImgInfor);
    }
    disconnect(this, SIGNAL(sendImgCenter(cv::Mat, INFOR_BASE::sImgInfor)),openimg,SLOT(getImgCenter(cv::Mat, INFOR_BASE::sImgInfor)));
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
    connect(this, SIGNAL(sendGrayRGB()), m_openimg, SLOT(Gray2RGB()));
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
}

void CentralWidget::EqualizationSlot()
{
    connect(this, SIGNAL(sendEqualizationSlot()), m_openimg, SLOT(EqualizationSlot()));
    emit sendEqualizationSlot();
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
