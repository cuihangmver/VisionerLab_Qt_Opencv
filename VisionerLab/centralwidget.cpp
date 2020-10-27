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
    btn1 ->setText("1");
    btn2 ->setText("2");
    btn3 ->setText("3");
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
    //delete m_parentCopy;
    delete btn1;
    delete btn2;
    delete btn3;
    delete qButtonHor;
    delete qButtonVer;
    delete m_textedit;
    delete m_openimg;
    delete ui;
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

        // INFOR_BASE::sImgInfor imgInfor;
        m_ImgInfor.sPath = name;
        m_ImgInfor.nWidth = image.rows;
        m_ImgInfor.nHeight = image.cols;
        m_ImgInfor.sColorSpace = "RGB";
        // 放在构造函数里信号传不过去
        //Form *openimg = new Form(this);
        openimg = new Form(this);
        m_openimg = openimg;
        // openimg = new Form(this);
        // FormPaint *openimg = new FormPaint(this);
        //connect(this, SIGNAL(sendImgCenter(cv::Mat, INFOR_BASE::sImgInfor)),openimg,SLOT(getImgCenter(cv::Mat, INFOR_BASE::sImgInfor)));
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

    // 放在构造函数里信号传不过去
    // Form *openimg = new Form(this);
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

    openimg->show();
}

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
