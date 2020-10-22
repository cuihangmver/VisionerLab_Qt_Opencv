#ifndef DIALOGSLIDER_H
#define DIALOGSLIDER_H

#include <QDialog>
#include <QSpinBox>
#include <QSlider>
#include <QBoxLayout>
namespace Ui {
class DialogSlider;
}

class DialogSlider : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSlider(QWidget *parent = nullptr);
    ~DialogSlider();
private slots:
    void ManualThresholdChangeSlot(int nValue);
private:
    Ui::DialogSlider *ui;
    QWidget *m_parent;
signals:
    void sendManualThresholdChange(int );
};

#endif // DIALOGSLIDER_H
