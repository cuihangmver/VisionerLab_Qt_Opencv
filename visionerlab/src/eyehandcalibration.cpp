#include "eyehandcalibration.h"
#include "ui_eyehandcalibration.h"

EyeHandCalibration::EyeHandCalibration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EyeHandCalibration)
{
    m_pParent = parent;
    pLabel1 = new QLabel(this);
    pLabel2 = new QLabel(this);

    lineEdit1 = new QLineEdit(this);
    lineEdit2 = new QLineEdit(this);

    qButtonHor1 = new QHBoxLayout;
    qButtonHor2 = new QHBoxLayout;

    qButtonVer = new QVBoxLayout;
    m_pOpenTCP = new QPushButton("Open");
    m_pOpenEXtrinsicCam = new QPushButton("Open");
    pLabel1->setText("TCP");
    pLabel2->setText("ExtrinsicCam");


    qButtonHor1->addWidget(pLabel1);
    qButtonHor1->addWidget(lineEdit1);
    qButtonHor1->addWidget(m_pOpenTCP);
    qButtonHor2->addWidget(pLabel2);
    qButtonHor2->addWidget(lineEdit2);
    qButtonHor2->addWidget(m_pOpenEXtrinsicCam);
    qButtonVer->addLayout(qButtonHor1);
    qButtonVer->addLayout(qButtonHor2);


    button = new QDialogButtonBox(parent);
    button->addButton( "Calibrate", QDialogButtonBox::YesRole);
    //button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), this, SLOT(OKCalParam()));
    connect(m_pOpenTCP, SIGNAL(clicked()), this, SLOT(OpenPathTCP()));
    connect(m_pOpenEXtrinsicCam, SIGNAL(clicked()), this, SLOT(OpenPathECam()));
    ui->setupUi(this);
    qButtonVer->addWidget(button);
    this->setLayout(qButtonVer);
}

EyeHandCalibration::~EyeHandCalibration()
{

}

void EyeHandCalibration::OKCalParam()
{
    qsLineEditText1 = lineEdit1->text();
    qsLineEditText2 = lineEdit2->text();

    connect(this, SIGNAL(EyeHandCalibrationParamSend(std::string ,std::string)), m_pParent, SLOT(EyeHandCalibrationParamSlot(std::string ,std::string)));
    qDebug() << qsLineEditText1;
    qDebug() << qsLineEditText2;
    std::string sPath1 = qsLineEditText1.toStdString();
    std::string sPath2 = qsLineEditText2.toStdString();
    emit EyeHandCalibrationParamSend(sPath1 ,sPath2);
    disconnect(this, SIGNAL(EyeHandCalibrationParamSend(std::string ,std::string)), m_pParent, SLOT(EyeHandCalibrationParamSlot(std::string ,std::string)));
}

void EyeHandCalibration::CancelCalParam()
{

}
void EyeHandCalibration::OpenPathTCP()
{
    QString file_full, file_name, file_path;
    QFileInfo fi;

    file_full = QFileDialog::getOpenFileName(this);

    fi = QFileInfo(file_full);
    file_name = fi.fileName();
    file_path = fi.absolutePath();
    lineEdit1->setText(file_path+"/"+file_name);
}
void EyeHandCalibration::OpenPathECam()
{
    QString file_full, file_name, file_path;
    QFileInfo fi;

    file_full = QFileDialog::getOpenFileName(this);

    fi = QFileInfo(file_full);
    file_name = fi.fileName();
    file_path = fi.absolutePath();
    lineEdit2->setText(file_path+"/"+file_name);
}
void EyeHandCalibration::closeEvent(QCloseEvent *event)
{
    this->close();
}
