#include "hitmiss.h"
#include "ui_hitmiss.h"

HitMiss::HitMiss(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HitMiss)
{
    m_qButtonVer = new QVBoxLayout;
    m_qSliderHor1 = new QHBoxLayout;
    m_qSliderHor2 = new QHBoxLayout;
    m_qSliderHor3 = new QHBoxLayout;
    m_pLabel = new QLabel(this);
    m_pLabel->setText("Level");
    m_parent = parent;
    int nMin = 3;
    int nMax = 29;
    int nSingleStep = 2;
    // 微调框
    m_pSpinBox = new QSpinBox(this);
    m_pSpinBox->setMinimum(nMin);  // 最小值
    m_pSpinBox->setMaximum(nMax);  // 最大值
    m_pSpinBox->setSingleStep(nSingleStep);  // 步长


    // 滑动条
    m_pSlider = new QSlider(this);
    m_pSlider->setOrientation(Qt::Horizontal);  // 水平方向
    m_pSlider->setMinimum(nMin);  // 最小值
    m_pSlider->setMaximum(nMax);  // 最大值
    m_pSlider->setSingleStep(nSingleStep);  // 步长
    m_qSliderHor1->addWidget(m_pLabel);
    m_qSliderHor1->addWidget(m_pSpinBox);
    m_qSliderHor1->addWidget(m_pSlider);

    pLabel1 = new QLabel(this);
    pLabel1->setText("StructureType");

    pComboBox1 = new QComboBox(this);
    pComboBox1->addItems({"RECT","CROSS","ELLIPSE"});
    m_qSliderHor2->addWidget(pLabel1);
    m_qSliderHor2->addWidget(pComboBox1);
    connect(pComboBox1, SIGNAL(currentIndexChanged(QString)), this, SLOT(SetStructureType(QString)));
    // 连接信号槽（相互改变）
    connect(m_pSpinBox, SIGNAL(valueChanged(int)), m_pSlider, SLOT(setValue(int)));
    connect(m_pSlider, SIGNAL(valueChanged(int)), m_pSpinBox, SLOT(setValue(int)));
    connect(m_pSpinBox, SIGNAL(valueChanged(int)), this, SLOT(ManualHitMissChangeSlot(int)));
    connect(m_pSlider, SIGNAL(valueChanged(int)), this, SLOT(ManualHitMissChangeSlot(int)));
    m_pSpinBox->setValue(5);
    button = new QDialogButtonBox(parent);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), parent, SLOT(OKHitMissSliderSelectImg()));
    connect(button, SIGNAL(rejected()), parent, SLOT(CancelHitMissSliderSelectImg()));

    m_qSliderHor3->addWidget(button);
    m_qButtonVer->addLayout(m_qSliderHor1);
    m_qButtonVer->addLayout(m_qSliderHor2);
    m_qButtonVer->addLayout(m_qSliderHor3);
    this->setLayout(m_qButtonVer);
    ui->setupUi(this);
}

HitMiss::~HitMiss()
{

}
void HitMiss::ManualHitMissChangeSlot(int nValue)
{
    connect(this, SIGNAL(sendManualHitMissChange(int , QString)), m_parent, SLOT(ManualHitMissChangeSlot(int, QString)));
    emit sendManualHitMissChange(nValue, m_StructureType);
    // 不加断开链接，执行第次越多，后面执行次数越多
    disconnect(this, 0, m_parent, 0);
}
void HitMiss::SetStructureType(QString StructureType)
{
    m_StructureType = StructureType;
}
void HitMiss::closeEvent(QCloseEvent *event)
{
    this->close();
}
