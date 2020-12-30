#include "cstructuredlightcalibration.h"
#include "ui_cstructuredlightcalibration.h"

CStructuredLightCalibration::CStructuredLightCalibration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CStructuredLightCalibration)
{
    m_pParent = parent;
    pLabel1 = new QLabel(this);
    pLabel2 = new QLabel(this);
    pLabel3 = new QLabel(this);
    pLabel4 = new QLabel(this);
    pLabel5 = new QLabel(this);
    pLabel6 = new QLabel(this);
    lineEdit1 = new QLineEdit(this);
    lineEdit2 = new QLineEdit(this);
    lineEdit3 = new QLineEdit(this);
    lineEdit4 = new QLineEdit(this);
    lineEdit5 = new QLineEdit(this);
    lineEdit6 = new QLineEdit(this);
    qButtonHor1 = new QHBoxLayout;
    qButtonHor2 = new QHBoxLayout;
    qButtonHor3 = new QHBoxLayout;
    qButtonHor4 = new QHBoxLayout;
    qButtonHor5 = new QHBoxLayout;
    qButtonHor6 = new QHBoxLayout;
    qButtonVer = new QVBoxLayout;
    m_pOpenPath = new QPushButton("Open");
    pLabel1->setText("Path");
    pLabel2->setText("dx");
    pLabel3->setText("2LineDistance");
    pLabel4->setText("PerMove");
    pLabel5->setText("width");
    pLabel6->setText("height");

    qButtonHor1->addWidget(pLabel1);
    qButtonHor1->addWidget(lineEdit1);
    qButtonHor1->addWidget(m_pOpenPath);
    qButtonHor2->addWidget(pLabel2);
    qButtonHor2->addWidget(lineEdit2);
    qButtonHor3->addWidget(pLabel3);
    qButtonHor3->addWidget(lineEdit3);
    qButtonHor4->addWidget(pLabel4);
    qButtonHor4->addWidget(lineEdit4);
    qButtonHor5->addWidget(pLabel5);
    qButtonHor5->addWidget(lineEdit5);
    qButtonHor6->addWidget(pLabel6);
    qButtonHor6->addWidget(lineEdit6);
    qButtonVer->addLayout(qButtonHor1);
    qButtonVer->addLayout(qButtonHor2);
    qButtonVer->addLayout(qButtonHor3);
    qButtonVer->addLayout(qButtonHor4);
    qButtonVer->addLayout(qButtonHor5);
    qButtonVer->addLayout(qButtonHor6);

    button = new QDialogButtonBox(parent);
    button->addButton( "Calibrate", QDialogButtonBox::YesRole);
    //button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), this, SLOT(OKCalParam()));
    connect(m_pOpenPath, SIGNAL(clicked()), this, SLOT(OpenPath()));
    ui->setupUi(this);
    qButtonVer->addWidget(button);
    this->setLayout(qButtonVer);
}

CStructuredLightCalibration::~CStructuredLightCalibration()
{

}

void CStructuredLightCalibration::OKCalParam()
{
    qsLineEditText1 = lineEdit1->text();
    qsLineEditText2 = lineEdit2->text();
    qsLineEditText3 = lineEdit3->text();
    qsLineEditText4 = lineEdit4->text();
    qsLineEditText5 = lineEdit5->text();
    qsLineEditText6 = lineEdit6->text();
    connect(this, SIGNAL(Structured_LightParamSend(std::string ,cv::Point ,double ,double ,double)), m_pParent, SLOT(Structured_LightParamSlot(std::string ,cv::Point ,double ,double ,double)));
    cv::Point pWidthHeight;
    pWidthHeight.x = qsLineEditText5.toInt();
    pWidthHeight.y = qsLineEditText6.toInt();
    std::string sPath = qsLineEditText1.toStdString();
    double dPerMove = qsLineEditText4.toDouble();
    double dDis2Line = qsLineEditText3.toDouble();
    double dC = qsLineEditText2.toDouble();
    qDebug() << qsLineEditText1;
    qDebug() << dPerMove;
    qDebug() << dDis2Line;
    qDebug() << dC;
    qDebug() << pWidthHeight.x;
    qDebug() << pWidthHeight.y;
    emit Structured_LightParamSend(sPath ,pWidthHeight ,dPerMove ,dDis2Line ,dC);
    disconnect(this, SIGNAL(Structured_LightParamSend(std::string ,cv::Point ,double ,double ,double)), m_pParent, SLOT(Structured_LightParamSlot(std::string ,cv::Point ,double ,double ,double)));
}

void CStructuredLightCalibration::CancelCalParam()
{

}
void CStructuredLightCalibration::OpenPath()
{
    QString file_full, file_name, file_path;
    QFileInfo fi;

    file_full = QFileDialog::getOpenFileName(this);

    fi = QFileInfo(file_full);
    file_name = fi.fileName();
    file_path = fi.absolutePath();
    lineEdit1->setText(file_path+file_name);
}
void CStructuredLightCalibration::closeEvent(QCloseEvent *event)
{
    this->close();
}
