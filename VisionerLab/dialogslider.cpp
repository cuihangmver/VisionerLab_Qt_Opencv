#include "dialogslider.h"
#include "ui_dialogslider.h"

DialogSlider::DialogSlider(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogSlider)
{
    ui->setupUi(this);
    m_parent = parent;
    int nMin = 0;
    int nMax = 255;
    int nSingleStep = 10;

    // QHBoxLayout *qSliderHor = new QHBoxLayout;
    qButtonVer = new QVBoxLayout;
    qSliderHor1 = new QHBoxLayout;
    pLabel1 = new QLabel(this);
    pLabel1->setText("Level");
    // 微调框
    // QSpinBox *pSpinBox = new QSpinBox(this);
    pSpinBox = new QSpinBox(this);
    pSpinBox->setMinimum(nMin);  // 最小值
    pSpinBox->setMaximum(nMax);  // 最大值
    pSpinBox->setSingleStep(nSingleStep);  // 步长

    // 滑动条
    // QSlider *pSlider = new QSlider(this);
    pSlider = new QSlider(this);
    pSlider->setOrientation(Qt::Horizontal);  // 水平方向
    pSlider->setMinimum(nMin);  // 最小值
    pSlider->setMaximum(nMax);  // 最大值
    pSlider->setSingleStep(nSingleStep);  // 步长
    qSliderHor1->addWidget(pLabel1);
    qSliderHor1->addWidget(pSpinBox);
    qSliderHor1->addWidget(pSlider);
    //this->setLayout(qSliderHor1);
    // 连接信号槽（相互改变）
    connect(pSpinBox, SIGNAL(valueChanged(int)), pSlider, SLOT(setValue(int)));
    connect(pSlider, SIGNAL(valueChanged(int)), pSpinBox, SLOT(setValue(int)));
    connect(pSpinBox, SIGNAL(valueChanged(int)), this, SLOT(ManualThresholdChangeSlot(int)));
    connect(pSlider, SIGNAL(valueChanged(int)), this, SLOT(ManualThresholdChangeSlot(int)));
    pSpinBox->setValue(125);

    qSliderHor2 = new QHBoxLayout;
    pLabel2 = new QLabel(this);
    pLabel2->setText("Negative");
    pCheckBox1 = new QCheckBox(this);
    qSliderHor2->addWidget(pLabel2);
    qSliderHor2->addWidget(pCheckBox1);

    qSliderHor3 = new QHBoxLayout;
    pLabel3 = new QLabel(this);
    pLabel3->setText("Preview");
    pCheckBox2 = new QCheckBox(this);
    qSliderHor3->addWidget(pLabel3);
    qSliderHor3->addWidget(pCheckBox2);


    qSliderHor4 = new QHBoxLayout;
    /*
    button1 = new QPushButton;
    button2 = new QPushButton;
    button1->setText("确认");
    button2->setText("取消");
    */
    QDialogButtonBox *button = new QDialogButtonBox(parent);
    button->addButton( "OK", QDialogButtonBox::YesRole);
    button->addButton( "NO", QDialogButtonBox::NoRole);
    connect(button, SIGNAL(accepted()), parent, SLOT(OKSelectImg()));
    connect(button, SIGNAL(rejected()), parent, SLOT(CancelSelectImg()));

    qSliderHor4->addWidget(button);
    //qSliderHor4->addWidget(button2);

    qButtonVer->addLayout(qSliderHor1);
    qButtonVer->addLayout(qSliderHor2);
    qButtonVer->addLayout(qSliderHor3);
    qButtonVer->addLayout(qSliderHor4);
    this->setLayout(qButtonVer);
}

DialogSlider::~DialogSlider()
{
    //delete m_parent;
    delete button1;
    delete button2;
    delete pLabel1;
    delete pLabel2;
    delete pLabel3;

    delete qSliderHor1;
    delete qSliderHor2;
    delete qSliderHor3;
    delete qSliderHor4;
    delete qButtonVer;
    delete pSpinBox;
    delete pSlider;
    delete ui;
}

void DialogSlider::ManualThresholdChangeSlot(int nValue)
{
    connect(this, SIGNAL(sendManualThresholdChange(int )), m_parent, SLOT(ManualThresholdChangeSlot(int)));
    emit sendManualThresholdChange(nValue);
    // 不加断开链接，执行第次越多，后面执行次数越多
    disconnect(this, 0, m_parent, 0);
}
