#include "connectedregion.h"
#include "ui_connectedregion.h"

ConnectedRegion::ConnectedRegion(QWidget *parent, int nRegion) :
    QDialog(parent),
    ui(new Ui::ConnectedRegion)
{

    pLabel1 = new QLabel(this);
    pComboBox1 = new QComboBox(this);
    m_qSliderHor1 = new QHBoxLayout;
    m_qSliderHor2 = new QHBoxLayout;
    m_qSliderHor3 = new QHBoxLayout;
    m_qButtonVer = new QVBoxLayout;
    m_pLabel = new QLabel(this);
    m_pLabel->setText("Level");
    m_parent = parent;

    m_qSliderHor1->addWidget(pLabel1);

    pLabel1->setText("4-8-connected components");
    pLabel1->setAlignment(Qt::AlignCenter);

    pComboBox1->addItems({"4","8"});

    m_qSliderHor1->addWidget(pComboBox1);

    connect(pComboBox1, SIGNAL(currentIndexChanged(QString)), this, SLOT(SetConnectedComponents48Slot(QString)));

    int nMin = 0;
    int nMax = nRegion;
    int nSingleStep = 1;
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

    m_qSliderHor2->addWidget(m_pLabel);
    m_qSliderHor2->addWidget(m_pSpinBox);
    m_qSliderHor2->addWidget(m_pSlider);

    // 连接信号槽（相互改变）
    connect(m_pSpinBox, SIGNAL(valueChanged(int)), m_pSlider, SLOT(setValue(int)));
    connect(m_pSlider, SIGNAL(valueChanged(int)), m_pSpinBox, SLOT(setValue(int)));
    connect(m_pSpinBox, SIGNAL(valueChanged(int)), this, SLOT(ManualConnectedRegionChangeSlot(int)));
    connect(m_pSlider, SIGNAL(valueChanged(int)), this, SLOT(ManualConnectedRegionChangeSlot(int)));
    m_pSpinBox->setValue(0);
    button = new QDialogButtonBox(parent);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), m_parent, SLOT(OKConnected_RegionSliderSelectImg()));
    connect(button, SIGNAL(rejected()), m_parent, SLOT(CancelConnected_RegionSliderSelectImg()));

    m_qSliderHor3->addWidget(button);
    m_qButtonVer->addLayout(m_qSliderHor1);
    m_qButtonVer->addLayout(m_qSliderHor2);
    m_qButtonVer->addLayout(m_qSliderHor3);
    this->setLayout(m_qButtonVer);
    ui->setupUi(this);
}

ConnectedRegion::~ConnectedRegion()
{
    delete pLabel1;
    delete pComboBox1;
    delete m_parent;
    delete m_pSpinBox;
    delete m_pSlider;
    delete m_pLabel;
    delete m_qButtonVer;
    delete m_qSliderHor1;
    delete m_qSliderHor2;
    delete m_qSliderHor3;
    delete button;
    delete ui;
}
void ConnectedRegion::ManualConnectedRegionChangeSlot(int nValue)
{
    connect(this, SIGNAL(sendManualConnectedRegionChange(int , int)), m_parent, SLOT(ManualConnected_RegionSlot(int, int)));
    emit sendManualConnectedRegionChange(nValue, m_nConnectedComponents48Value);
    // 不加断开链接，执行第次越多，后面执行次数越多
    disconnect(this, SIGNAL(sendManualConnectedRegionChange(int , int)), m_parent, SLOT(ManualConnected_RegionSlot(int, int)));
}
void ConnectedRegion::SetConnectedComponents48Slot(QString sConnectedComponents48Value)
{
    m_nConnectedComponents48Value = sConnectedComponents48Value.toInt();
}

