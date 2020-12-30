#ifndef FORMNOTVISUALIZED_H
#define FORMNOTVISUALIZED_H

#include <QWidget>
#include <QMainWindow>
#include <QDialog>
#include <QDebug>
#include "ccalibration.h"
class FormNotVisualized
{
public:
    FormNotVisualized();
private slots:
    void ManualStructured_LightSlot();
    void ManualStructured_LightParamSlot();
};

#endif // FORMNOTVISUALIZED_H
