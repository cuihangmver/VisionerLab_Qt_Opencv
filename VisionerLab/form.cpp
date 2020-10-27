#include "form.h"
#include "ui_form.h"

Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    m_parentCopy = parent;
    // 给dialog增加最大化最小化
    Qt::WindowFlags flags = Qt::Dialog;
    flags |= Qt::WindowMinMaxButtonsHint;
    flags |= Qt::WindowCloseButtonHint;
    setWindowFlags(flags);
    // 设置鼠标移动相应
    this->setMouseTracking(true);
    // 传给mainwindow
    // connect(this,SIGNAL(sendMouse(int, int, QColor,int)), m_parentCopy, SLOT(getMouse(int, int, QColor, int)));
    // 传给centralwidget
    connect(this,SIGNAL(sendMouse(int, int, QColor,int)), m_parentCopy, SLOT(getMouse(int, int, QColor, int)));
    m_label = new QLabel;

    scrollArea = new QScrollArea(this);
    m_dScaling = 1.0;
    m_bCtrlPress = false;
    m_bModifyImg = false;
}



Form::~Form()
{

    delete pg;
    delete pv;
    delete m_dialogSlider;
    delete scrollArea;
    delete m_label;
    //delete m_parentCopy;
    delete ui;
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

void Form::DisplayMat(cv::Mat mimage, QLabel *label, double dScaling)
{
    // 缩放像素级别,这里的放大效果是像素级别，如果放大2倍，某一个像素现在变为4个
    cv::Mat image;
    //if(dScaling > 1)
    //{
    // 转换思路，直接全部图像放大会造成计算机内存占用过大，解决方法是对显示的图像进行局部放大并显示
        //Resize(mimage, image, dScaling);
    //}
    //else
    //{
        cv::resize(mimage,image,cv::Size(0,0),dScaling,dScaling,cv::INTER_AREA);
    //}
    QImage img;
    // Mat格式转QImage
    if(4 == image.channels())
    {
        qDebug() << "channels4";
        // BGRA转RGBA
        //img = QImage((const unsigned char*)(image.data),
        img = QImage(image.data,
                     image.cols,image.rows,image.step,//rgb.cols*rgb.channels()可以替换为image.step
                     QImage::Format_RGB32);
                     //QImage::Format_Grayscale8);
qDebug() << "channels5";
        connect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageCloseGraySlot(std::vector<std::string> ,std::vector<std::string>)));
        std::vector<std::string> vsShowButtons;
        std::vector<std::string> vsCloseButtons;
        std::string s1 = "actionManual";
        vsCloseButtons.push_back(s1);
        emit sendButtonShowManage(vsShowButtons, vsCloseButtons);
        disconnect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageCloseGraySlot(std::vector<std::string> ,std::vector<std::string>)));;
        /**/
    }
    else if(3 == image.channels())
    {
        // BGR转RGB
        img = QImage((const unsigned char*)(image.data),
                     image.cols,image.rows,image.cols*image.channels(),//rgb.cols*rgb.channels()可以替换为image.step
                     QImage::Format_RGB888);
                     //QImage::Format_Grayscale8);

        connect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageCloseGraySlot(std::vector<std::string> ,std::vector<std::string>)));
        std::vector<std::string> vsShowButtons;
        std::vector<std::string> vsCloseButtons;
        std::string s1 = "actionManual";
        vsCloseButtons.push_back(s1);
        emit sendButtonShowManage(vsShowButtons, vsCloseButtons);
        disconnect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageCloseGraySlot(std::vector<std::string> ,std::vector<std::string>)));;
        /**/
    }
    else if(1 == image.channels())
    {
        // 灰度图
        img = QImage((const unsigned char*)(image.data),
                     image.cols,image.rows,image.cols*image.channels(),
                     QImage::Format_Grayscale8);
        connect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageOpenGraySlot(std::vector<std::string> ,std::vector<std::string>)));
        std::vector<std::string> vsShowButtons;
        std::vector<std::string> vsCloseButtons;
        std::string s1 = "actionManual";
        vsShowButtons.push_back(s1);
        emit sendButtonShowManage(vsShowButtons, vsCloseButtons);
        disconnect(this, SIGNAL(sendButtonShowManage(std::vector<std::string> ,std::vector<std::string>)),m_parentCopy,SLOT(ButtonShowManageOpenGraySlot(std::vector<std::string> ,std::vector<std::string>)));
    }
qDebug() << "channels6";
    // QImage转QPixmap
    //QPixmap fitpixmap = QPixmap::fromImage(img).scaled(img.width(), img.height(),Qt::KeepAspectRatio, Qt::SmoothTransformation);
    // 需要使用copy()
    QPixmap fitpixmap = QPixmap::fromImage(img.copy());
qDebug() << "channels7";
    label->setPixmap(fitpixmap);
    qDebug() << "channels8";
    // QLabel居中
    label->setAlignment(Qt::AlignCenter);
    qDebug() << "channels9";
}

void Form::DisplayMatPainter(cv::Mat image, QLabel *label, double dScaling)
{
    cv::Mat rgb;
    QImage img;
    // Mat格式转QImage
    if(image.channels() == 3)
    {
        // BGR转RGB
        cvtColor(image,rgb,CV_BGR2RGB);
        img = QImage((const unsigned char*)(rgb.data),
                     rgb.cols,rgb.rows,rgb.cols*rgb.channels(),//rgb.cols*rgb.channels()可以替换为image.step
                     QImage::Format_RGB888);
    }
    else
    {
        // 灰度图
        img = QImage((const unsigned char*)(image.data),
                     image.cols,image.rows,rgb.cols*image.channels(),
                     QImage::Format_RGB888);
    }
}

void Form::getImg(cv::Mat mImg, INFOR_BASE::sImgInfor imgInfor)
{
    m_mImg = mImg.clone();
    m_imgInfor = imgInfor;
    //layout布局
    QPushButton *pb,*pb1,*pb2;
    pb=new QPushButton("放大");
    pb1=new QPushButton("缩小");
    pb2=new QPushButton("恢复");
    QHBoxLayout *pg=new QHBoxLayout;
    QVBoxLayout *pv=new QVBoxLayout;
    pg->addWidget(pb);
    pg->addWidget(pb1);
    pg->addWidget(pb2);

    // 设置滚动条相关
    scrollArea->setWidget(m_label);
    //scrollArea->setBackgroundRole(QPalette::Dark);
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
    DisplayMat(mImg, m_label, m_dScaling);

    // 这两个控件都要相应鼠标事件
    m_label->setMouseTracking(true);
    scrollArea->setMouseTracking(true);

    pv->addWidget(scrollArea);
    pv->addLayout(pg,1);
    this->setLayout(pv);
    this->resize(m_mImg.cols,m_mImg.rows);
    this->show();
}

void Form::getImgCenter(cv::Mat mImg, INFOR_BASE::sImgInfor imgInfor)
{
    m_mImg = mImg.clone();
    m_imgInfor = imgInfor;
    /*layout布局*/
    QPushButton *pb,*pb1,*pb2;
    pb=new QPushButton("放大");
    pb1=new QPushButton("缩小");
    pb2=new QPushButton("恢复");
    // QHBoxLayout *pg=new QHBoxLayout;
    // QVBoxLayout *pv=new QVBoxLayout;
    pg = new QHBoxLayout;
    pv = new QVBoxLayout;
    pg->addWidget(pb);
    pg->addWidget(pb1);
    pg->addWidget(pb2);

    // 设置滚动条相关
    scrollArea->setWidget(m_label);
    //scrollArea->setBackgroundRole(QPalette::Dark);
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
    DisplayMat(mImg, m_label, m_dScaling);

    // 这两个控件都要相应鼠标事件
    m_label->setMouseTracking(true);
    scrollArea->setMouseTracking(true);

    pv->addWidget(scrollArea);
    pv->addLayout(pg,1);
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
            int nChannel = m_mImg.channels();
            emit sendMouse((ptLabel.x() - xoffset) / m_dScaling, (ptLabel.y() - yoffset-1) / m_dScaling, color, nChannel);
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

void Form::keyPressEvent(QKeyEvent *ev)
{
    if(ev->modifiers() == Qt::ControlModifier)
    {
       m_bCtrlPress = true;
       scrollArea->setAttribute(Qt::WA_TransparentForMouseEvents, true);
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
        qApp->quit();
    }
    QString filepath = QString::fromStdString(m_imgInfor.sPath);

    int ret = QMessageBox::warning(0,"Warning",QString("%1 has been changed!\n Do you want to save it?").arg(filepath),QMessageBox::Yes |QMessageBox::No|QMessageBox::Cancel,QMessageBox::No);
    if(ret == QMessageBox::Yes) //如果选择保存文件，则执行保存操作
    {
        std::string s = filepath.toStdString();
        cv::imwrite(s, m_mImg);
        //doFileSave();//保存文件
    }
    else if(ret == QMessageBox::Cancel)
    {
        // event->ignore();
    }
    else
    {
        //qApp->quit();//再退出系统，qApp是指向应用程序的全局指针
    }
}

void Form::closeEventSlot()
{
    if(!m_bModifyImg)
    {
        qApp->quit();
    }
    QString filepath = QString::fromStdString(m_imgInfor.sPath);

    int ret = QMessageBox::warning(0,"Warning",QString("%1 has been changed!\n Do you want to save it?").arg(filepath),QMessageBox::Yes |QMessageBox::No|QMessageBox::Cancel,QMessageBox::No);
    if(ret == QMessageBox::Yes) //如果选择保存文件，则执行保存操作
    {
        std::string s = filepath.toStdString();
        cv::imwrite(s, m_mImg);
        //doFileSave();//保存文件
    }
    else if(ret == QMessageBox::Cancel)
    {
        qApp->quit();;
    }
    else
    {
        qApp->quit();//再退出系统，qApp是指向应用程序的全局指针
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
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::Gray2RGB()
{
    m_bModifyImg = true;
    if(1 == m_mImg.channels())
    {
        cv::cvtColor(m_mImg,m_mImg,CV_GRAY2RGB);
    }
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::ManualThresholdSlot()
{
    m_dialogSlider = new DialogSlider(this);
    m_dialogSlider->show();
}
void Form::ManualThresholdChangeSlot(int nValue)
{
    cv::threshold(m_mImg, m_mTem, nValue, 255, cv::THRESH_BINARY_INV);
    DisplayMat(m_mTem, m_label, m_dScaling);
}

void Form::ThresholdOtusSlot()
{
    cv::threshold(m_mImg, m_mImg, 0, 255, CV_THRESH_OTSU);
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::ThresholdAdaptiveSlot()
{
    m_pAdaptiveDialog = new AdaptiveDialog(this);
    m_pAdaptiveDialog->show();
}

void Form::ThresholdAdaptiveChangeSlot(double dMaxValue, int nAdaptiveMethod, int nThresholdType, int nBlockSize, double dC)
{
    cv::adaptiveThreshold(m_mImg, m_mImg, dMaxValue, nAdaptiveMethod, nThresholdType, nBlockSize, dC);
    DisplayMat(m_mImg, m_label, m_dScaling);
}

void Form::OKSelectImg()
{
    m_mImg = m_mTem.clone();
    DisplayMat(m_mImg, m_label, m_dScaling);
}
void Form::CancelSelectImg()
{
    DisplayMat(m_mImg, m_label, m_dScaling);

}
