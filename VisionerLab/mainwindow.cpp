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

    // openimg = new Form(this);
    // button的信号是clicked(),QAction的信号是triggered()
    QObject::connect(ui->actionNew, SIGNAL(triggered()), this,SLOT(OpenNew()));
    // 图像处理槽函数
    connect(ui->actionGray, SIGNAL(triggered()), c, SLOT(RGB2Gray()));
    connect(ui->actionRGB, SIGNAL(triggered()), c, SLOT(Gray2RGB()));
    connect(ui->actionsave, SIGNAL(triggered()), c,SLOT(closeEventSlot()));
    connect(ui->actionsave_2, SIGNAL(triggered()), c,SLOT(closeEventSaveAsSlot()));
    connect(ui->actionManual, SIGNAL(triggered()), c,SLOT(ManualThresholdSlot()));
    initButton(ui);
    m_ui = ui;
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::getCentralWidget(CentralWidget *cenwidget)
{
    cenwidget = c;
}
void MainWindow::initButton(Ui::MainWindow *ui)
{
    // ui->actionNew->setDisabled(true);
    // ui->actionOpen->setDisabled(true);
    ui->actionRGB->setDisabled(true);
    ui->actionGray->setDisabled(true);
    ui->actionMap->setDisabled(true);
    ui->actionsave->setDisabled(true);
    ui->actionsave_2->setDisabled(true);
    ui->actionBinary->setDisabled(true);
    ui->actionManual->setDisabled(true);
}

void MainWindow::openButton(Ui::MainWindow *ui)
{
    // ui->actionNew->setDisabled(true);
    // ui->actionOpen->setDisabled(true);
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
        // openButton(m_ui);
        m_ImgInfor.sPath = name;
        m_ImgInfor.nWidth = image.rows;
        m_ImgInfor.nHeight = image.cols;
        m_ImgInfor.sColorSpace = "RGB";
        connect(this,SIGNAL(sendImg(cv::Mat, INFOR_BASE::sImgInfor)),c,SLOT(OpenNew(cv::Mat, INFOR_BASE::sImgInfor)));
        emit sendImg(image, m_ImgInfor);
    }
}

void MainWindow::ButtonShowManage(std::vector<std::string> vsShowButtons, std::vector<std::string> vsCloseButtons)
{
    std::string s1 = "actionRGB";
    std::string s2 = "actionGray";
    std::string s3 = "actionMap";
    std::string s4 = "actionsave";
    std::string s5 = "actionsave_2";
    std::string s6 = "actionBinary";
    std::string s7 = "actionManual";
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
    }
}

void MainWindow::on_actionOpen_triggered()
{
    /* 打开txt文件
    QString fileName;
    // 选择单个文件，选择多个文件用getOpenFileNames
    fileName = QFileDialog::getOpenFileName(this, "Open File", "./", "Text File(*.txt)");
    if("" == fileName)
    {
        return ;
    }
    else
    {
        QFile file(fileName);

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QMessageBox::warning(this,"error","open file error!");
            return;
        }
        else
        {
            if(!file.isReadable())
                QMessageBox::warning(this,"error","this file is not readable!");
            else
            {
                // 将打开的文本显示在textEdit中
                QTextStream textStream(&file);
                while(!textStream.atEnd())
                {
                    ui->textEdit->setPlainText(textStream.readAll());
                }
                ui->textEdit->show();
                file.close();
                //flag_isOpen = 1;
                //Last_FileName = fileName;
            }

        }
    }
    */
    // 打开图像
    QString fileNameImg = QFileDialog::getOpenFileName(this, tr("Open Image"),"./",tr("Image File (*.jpg *.png *.bmp)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(fileNameImg).data();//filename.toAscii().data()
    cv::Mat image = cv::imread(name);
    if(!image.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Image Data Is Null"));
        msgBox.exec();
    }
    else
    {
        //INFOR_BASE::sImgInfor imgInfor;
        m_ImgInfor.sPath = name;
        m_ImgInfor.nWidth = image.rows;
        m_ImgInfor.nHeight = image.cols;
        m_ImgInfor.sColorSpace = "RGB";
        //dialog = new Dialog(this);
        //dialog->setModal(false);
        //QObject::connect(this,SIGNAL(sendImg(cv::Mat, INFOR_BASE::sImgInfor)),dialog,SLOT(getImg(cv::Mat, INFOR_BASE::sImgInfor)));
        //emit sendImg(image, m_ImgInfor);
        //dialog->show();
    }
}

