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
    qSliderHor = new QHBoxLayout;
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
    qSliderHor->addWidget(pSpinBox);
    qSliderHor->addWidget(pSlider);
    this->setLayout(qSliderHor);
    // 连接信号槽（相互改变）
    connect(pSpinBox, SIGNAL(valueChanged(int)), pSlider, SLOT(setValue(int)));
    connect(pSlider, SIGNAL(valueChanged(int)), pSpinBox, SLOT(setValue(int)));
    connect(pSpinBox, SIGNAL(valueChanged(int)), this, SLOT(ManualThresholdChangeSlot(int)));
    connect(pSlider, SIGNAL(valueChanged(int)), this, SLOT(ManualThresholdChangeSlot(int)));
    pSpinBox->setValue(10);

}

DialogSlider::~DialogSlider()
{
    //delete m_parent;
    delete qSliderHor;
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
