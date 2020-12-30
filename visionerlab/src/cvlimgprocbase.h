//*************************************
/*
 * 图像处理函数库，包含一些常用的图像处理方法。
                                     */
//*************************************
#ifndef CVLIMGPROCBASE_H
#define CVLIMGPROCBASE_H

#include <opencv2/opencv.hpp>
#include <windows.h>
#include <QDebug>
#include "inforbase.h"
 #include <climits>
using namespace INFOR_BASE;
class CVLImgProcBase
{
public:
    CVLImgProcBase();
    ~CVLImgProcBase();
public:
    // 判断是否为二值图，是true，否false
    bool IfBinaryImg(cv::Mat mImg);
    // RANSAC拟合二维平面直线
    // vp3dLine：待拟合数据，z值无效
    // nNumItera：迭代次数
    // dTMin：有效点距离直线最小距离
    // dNumPercentageT：有效点比例阈值
    S_2D_LINE FitLineRansac(std::vector<cv::Point3d> vp3dLine, int nNumItera, double dTMin, double dNumPercentageT);
    // 最小二乘法拟合直线
    S_2D_LINE FitLine(std::vector<cv::Point3d> vp3dLine);
    // 点到直线距离
    double DistPointToLine(cv::Point3d pt, S_2D_LINE cLineParam);
    // x，y换位置
    void SwapXY(std::vector<cv::Point3d> &vp3dLine);
    // 得到直线方程
    S_2D_LINE GetLineParam(cv::Point3d pt1, cv::Point3d pt2);
    // 得到点到直线的投影点
    cv::Point3d ProjectPointsToLine(cv::Point3d pt, S_2D_LINE cLineParam);
    // 二维平面上两点的欧式距离
    double Euler2DDistance(cv::Point3d pt1, cv::Point3d pt2);
};

#endif // CVLIMGPROCBASE_H
