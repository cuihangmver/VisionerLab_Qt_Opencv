#include "adaptivedialog.h"
#include "ui_adaptivedialog.h"

AdaptiveDialog::AdaptiveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AdaptiveDialog)
{
    ui->setupUi(this);
    m_pParent = parent;
    pLabel1 = new QLabel(this);
    pLabel2 = new QLabel(this);
    pLabel3 = new QLabel(this);
    pLabel4 = new QLabel(this);
    pLabel5 = new QLabel(this);

    pComboBox1 = new QComboBox(this);
    pComboBox2 = new QComboBox(this);
    pComboBox3 = new QComboBox(this);
    pComboBox4 = new QComboBox(this);
    pComboBox5 = new QComboBox(this);

    qButtonHor1 = new QHBoxLayout;
    qButtonHor2 = new QHBoxLayout;
    qButtonHor3 = new QHBoxLayout;
    qButtonHor4 = new QHBoxLayout;
    qButtonHor5 = new QHBoxLayout;

    qButtonVer = new QVBoxLayout;

    m_dMaxValue = 0;
    m_nAdaptiveMethod = cv::ADAPTIVE_THRESH_MEAN_C;
    m_nThresholdType = cv::THRESH_BINARY;
    m_nBlockSize = 3;
    m_dC = 20.0;
    qButtonHor1->addWidget(pLabel1);
    pLabel1->setText("MaxValue");
    pLabel1->setAlignment(Qt::AlignCenter);
    pComboBox1->addItems({"0","255"});
    qButtonHor1->addWidget(pComboBox1);
    connect(pComboBox1, SIGNAL(currentIndexChanged(QString)), this, SLOT(SetMaxValue(QString)));
    qButtonHor2->addWidget(pLabel2);
    pLabel2->setText("AdaptiveMethod");
    pLabel2->setAlignment(Qt::AlignCenter);
    pComboBox2->addItems({"ADAPTIVE_THRESH_MEAN_C","ADAPTIVE_THRESH_GAUSSIAN_C"});
    qButtonHor2->addWidget(pComboBox2);
    connect(pComboBox2, SIGNAL(currentIndexChanged(QString)), this, SLOT(SetAdaptiveMethod(QString)));
    qButtonHor3->addWidget(pLabel3);
    pLabel3->setText("ThresholdType");
    pLabel3->setAlignment(Qt::AlignCenter);
    pComboBox3->addItems({"THRESH_BINARY ","THRESH_BINARY_INV"});
    qButtonHor3->addWidget(pComboBox3);
    connect(pComboBox3, SIGNAL(currentIndexChanged(QString)), this, SLOT(SetThresholdType(QString)));
    qButtonHor4->addWidget(pLabel4);
    pLabel4->setText("BlockSize");
    pLabel4->setAlignment(Qt::AlignCenter);
    pComboBox4->addItems({"3 ","5", "7", "9",
                          "11 ","13", "15", "17",
                          "19 ","21", "23", "25",
                          "27 ","29", "31", "33",
                          "35 ","37", "39", "41"});
    qButtonHor4->addWidget(pComboBox4);
    connect(pComboBox4, SIGNAL(currentIndexChanged(QString)), this, SLOT(SetBlockSize(QString)));
    qButtonHor5->addWidget(pLabel5);
    pLabel5->setText("C");
    pLabel5->setAlignment(Qt::AlignCenter);
    pComboBox5->addItems({"20", "40"});
    qButtonHor5->addWidget(pComboBox5);
    connect(pComboBox5, SIGNAL(currentIndexChanged(QString)), this, SLOT(SetC(QString)));
    qButtonVer->addLayout(qButtonHor1);
    qButtonVer->addLayout(qButtonHor2);
    qButtonVer->addLayout(qButtonHor3);
    qButtonVer->addLayout(qButtonHor4);
    qButtonVer->addLayout(qButtonHor5);

    pb = new QPushButton("预览");
    qButtonVer->addWidget(pb);
    connect(pb, SIGNAL(clicked()), this, SLOT(PreviewSelectAdaptive()));
    button = new QDialogButtonBox(parent);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), this, SLOT(OKSelectAdaptive()));
    connect(button, SIGNAL(rejected()), this, SLOT(CancelSelectAdaptive()));

    qButtonVer->addWidget(button);
    this->setLayout(qButtonVer);
}

AdaptiveDialog::~AdaptiveDialog()
{
    if(nullptr != pLabel1)
    {
        delete pLabel1;
        pLabel1 = nullptr;
    }
    if(nullptr != pLabel2)
    {
        delete pLabel2;
        pLabel2 = nullptr;
    }
    if(nullptr != pLabel3)
    {
        delete pLabel3;
        pLabel3 = nullptr;
    }
    if(nullptr != pLabel4)
    {
        delete pLabel4;
        pLabel4 = nullptr;
    }
    if(nullptr != pLabel5)
    {
        delete pLabel5;
        pLabel5 = nullptr;
    }

    if(nullptr != pComboBox1)
    {
        delete pComboBox1;
        pComboBox1 = nullptr;
    }
    if(nullptr != pComboBox2)
    {
        delete pComboBox2;
        pComboBox2 = nullptr;
    }
    if(nullptr != pComboBox3)
    {
        delete pComboBox3;
        pComboBox3 = nullptr;
    }
    if(nullptr != pComboBox4)
    {
        delete pComboBox4;
        pComboBox4 = nullptr;
    }
    if(nullptr != pComboBox5)
    {
        delete pComboBox5;
        pComboBox5 = nullptr;
    }

    if(nullptr != qButtonHor1)
    {
        delete qButtonHor1;
        qButtonHor1 = nullptr;
    }
    if(nullptr != qButtonHor2)
    {
        delete qButtonHor2;
        qButtonHor2 = nullptr;
    }
    if(nullptr != qButtonHor3)
    {
        delete qButtonHor3;
        qButtonHor3 = nullptr;
    }
    if(nullptr != qButtonHor4)
    {
        delete qButtonHor4;
        qButtonHor4 = nullptr;
    }
    if(nullptr != qButtonHor5)
    {
        delete qButtonHor5;
        qButtonHor5 = nullptr;
    }

    if(nullptr != qButtonVer)
    {
        delete qButtonVer;
        qButtonVer = nullptr;
    }
    if(nullptr != button)
    {
        delete button;
        button = nullptr;
    }
    if(nullptr != pb)
    {
        delete pb;
        pb = nullptr;
    }
    if(nullptr != ui)
    {
        delete ui;
        ui = nullptr;
    }
}

void AdaptiveDialog::SetMaxValue(QString sMaxValue)
{
    m_dMaxValue = sMaxValue.toDouble();
}

void AdaptiveDialog::SetAdaptiveMethod(QString sAdaptiveMethod)
{
    m_nAdaptiveMethod = sAdaptiveMethod.toInt();
}
void AdaptiveDialog::SetThresholdType(QString sThresholdType)
{
    m_nThresholdType = sThresholdType.toInt();
}
void AdaptiveDialog::SetBlockSize(QString sBlockSize)
{
    m_nBlockSize = sBlockSize.toInt();
}
void AdaptiveDialog::SetC(QString sC)
{
    m_dC = sC.toDouble();
}
void AdaptiveDialog::OKSelectAdaptive()
{
    connect(this, SIGNAL(ThresholdAdaptiveChangeSend(double , int , int , int , double )), m_pParent, SLOT(ThresholdAdaptiveChangeSlot(double , int , int , int , double)));
    emit ThresholdAdaptiveChangeSend(m_dMaxValue , m_nAdaptiveMethod , m_nThresholdType , m_nBlockSize ,m_dC);
    disconnect(this, 0, m_pParent, 0);
}
void AdaptiveDialog::CancelSelectAdaptive()
{

}
void AdaptiveDialog::PreviewSelectAdaptive()
{
    connect(this, SIGNAL(PreviewThresholdAdaptiveChangeSend(double , int , int , int , double )), m_pParent, SLOT(PreviewThresholdAdaptiveChangeSlot(double , int , int , int , double)));
    emit PreviewThresholdAdaptiveChangeSend(m_dMaxValue , m_nAdaptiveMethod , m_nThresholdType , m_nBlockSize ,m_dC);
    disconnect(this, 0, m_pParent, 0);
}
