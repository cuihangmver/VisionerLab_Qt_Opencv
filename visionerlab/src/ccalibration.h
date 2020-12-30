#ifndef CCALIBRATION_H
#define CCALIBRATION_H
#include <QWidget>
#include <QMainWindow>
#include <QDialog>
#include <QDebug>
#include <opencv2/opencv.hpp>
class CCalibration
{
public:
    CCalibration();
public:
    void StructedLightReadData
    (
            std::string sPath,
            std::vector<cv::Point> &vpLeft,
            std::vector<cv::Point> &vpRight
    );
    void CCalibration::StructedLightCalibration
    (
            std::vector<cv::Point> vpLeft,
            std::vector<cv::Point> vpRight,
            cv::Point pWidthHeight,
            double dPerMove,
            double dDis2Line,
            double dC, // 像元大小，0.0022
            double &dCenterDis,
            double &dcot,
            double &dBaseLine,
            double &dF
    );
};

#endif // CCALIBRATION_H
