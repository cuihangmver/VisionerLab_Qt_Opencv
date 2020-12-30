#include "cvlimgprocbase.h"

CVLImgProcBase::CVLImgProcBase()
{

}
CVLImgProcBase::~CVLImgProcBase()
{

}
// 判断图像是否为二值图
bool CVLImgProcBase::IfBinaryImg(cv::Mat mImg)
{
    if(mImg.empty())
    {
        qDebug() << "Empty Image!";
        return false;
    }
    if(1 != mImg.channels())
    {
        qDebug() << "Not Gray Image!";
        return false;
    }
    if(2 == mImg.depth())
    {
        qDebug() << "Not 8Gray Image!";
        return false;
    }
    for(int nHeight = 0; nHeight < mImg.rows; nHeight++)
    {
        for(int nWidth = 0; nWidth < mImg.cols; nWidth++)
        {
            int nPiexl = mImg.at<uchar>(nHeight, nWidth);
            if((255 != nPiexl) && (0 != nPiexl))
            {
                qDebug() << "Not Binary Image!";
                return false;
            }
        }
    }
    return true;
}

// RANSAC拟合二维平面直线
// vp3dLine：待拟合数据，z值无效
// nNumItera：迭代次数
// dTMin：有效点距离直线最小距离
// dNumPercentageT：有效点比例阈值
S_2D_LINE CVLImgProcBase::FitLineRansac(std::vector<cv::Point3d> vp3dLine, int nNumItera, double dTMin, double dNumPercentageT)
{
    int nMaxNumPt2Line = 0;
    S_2D_LINE cMaxLine;

    srand((int)time(0));
    for (int i = 0; i < nNumItera; i++)
    {

        int nPoint1 = rand() % (vp3dLine.size()-1);
        Sleep(1);
        int nPoint2 = rand() % (vp3dLine.size()-1);
        S_2D_LINE cLine = GetLineParam(vp3dLine[nPoint1], vp3dLine[nPoint2]);

        int nNumShortT = 0;
        for (int j = 0; j < vp3dLine.size(); j++)
        {
            double dPoint2Line = DistPointToLine(vp3dLine[j], cLine);
            if (dPoint2Line < dTMin)
            {
                nNumShortT++;
            }
        }
        if (nNumShortT > nMaxNumPt2Line)
        {
            nMaxNumPt2Line = nNumShortT;
            cMaxLine = cLine;
        }
        if (nMaxNumPt2Line > dNumPercentageT*vp3dLine.size())
        {
            break;
        }
    }
    std::vector<cv::Point3d> vp3dFitLine;
    for (int i = 0; i < vp3dLine.size(); i++)
    {
        double dPoint2Line = DistPointToLine(vp3dLine[i], cMaxLine);
        if (dPoint2Line < dTMin)
        {
            vp3dFitLine.push_back(vp3dLine[i]);
        }
    }
    S_2D_LINE cRansacFitLine = FitLine(vp3dFitLine);
    return cRansacFitLine;
}


// 最小二乘法拟合直线
S_2D_LINE CVLImgProcBase::FitLine(std::vector<cv::Point3d> vp3dLine)
{
    S_2D_LINE cLineParam;
    if (vp3dLine.size() <= 0)
    {
        return cLineParam;
    }

    // Select Fit Method:
    // 1. y = k * x + b;
    // 2. x = k * y + b;！！！！！！！！！！！！两种线，坐标系颠倒

    int nXMin = INT_MAX;
    int nYMin = INT_MAX;
    int nXMax = INT_MIN;
    int nYMax = INT_MIN;
    int nIdx;

    for (nIdx = 0; nIdx < vp3dLine.size(); nIdx++)
    {
        if (vp3dLine[nIdx].x < nXMin) nXMin = vp3dLine[nIdx].x;
        if (vp3dLine[nIdx].y < nYMin) nYMin = vp3dLine[nIdx].y;
        if (vp3dLine[nIdx].x > nXMax) nXMax = vp3dLine[nIdx].x;
        if (vp3dLine[nIdx].y > nYMax) nYMax = vp3dLine[nIdx].y;
    }

    if (abs(nYMax - nYMin) > abs(nXMax - nXMin))
    {
        cLineParam._line_type = 0;
        SwapXY(vp3dLine);
    }
    else
    {
        cLineParam._line_type = 1;
    }

    double sigmaX, sigmaY, sigmaXX, sigmaXY;
    sigmaX = sigmaY = sigmaXX = sigmaXY = 0.0;

    for (nIdx = 0; nIdx < vp3dLine.size(); nIdx++)
    {
        sigmaX += vp3dLine[nIdx].x;
        sigmaY += vp3dLine[nIdx].y;
        sigmaXX += vp3dLine[nIdx].x * vp3dLine[nIdx].x;
        sigmaXY += vp3dLine[nIdx].x * vp3dLine[nIdx].y;
    }

    if (abs(nYMax - nYMin) > abs(nXMax - nXMin))
    {
        SwapXY(vp3dLine);
    }

    double tmp_denominator = (vp3dLine.size() * sigmaXX) - (sigmaX * sigmaX);
    if (fabs(tmp_denominator) < 1e-2)
    {
        return cLineParam;
    }
    else
    {
        if (((vp3dLine.size() * sigmaXX) - (sigmaX * sigmaX)) > 0)
        {
            cLineParam._k = (double)((vp3dLine.size() * sigmaXY) - (sigmaX * sigmaY)) /
                (double)((vp3dLine.size() * sigmaXX) - (sigmaX * sigmaX));
        }
        if (((vp3dLine.size() * sigmaXX) - (sigmaX * sigmaX)) > 0)
        {
            cLineParam._b = (double)((sigmaXX * sigmaY) - (sigmaXY * sigmaX)) /
                (double)((vp3dLine.size() * sigmaXX) - (sigmaX * sigmaX));
        }

        return cLineParam;
    }
    return cLineParam;
}

// x，y换位置
void CVLImgProcBase::SwapXY(std::vector<cv::Point3d> &vp3dLine)
{
    int nSwap = 0;

    int nIdx = 0;
    for (nIdx = 0; nIdx < vp3dLine.size(); nIdx++)
    {
        nSwap = vp3dLine[nIdx].x;
        vp3dLine[nIdx].x = vp3dLine[nIdx].y;
        vp3dLine[nIdx].y = nSwap;
    }
}

// 点到直线距离
double CVLImgProcBase::DistPointToLine(cv::Point3d pt, S_2D_LINE cLineParam)
{
    cv::Point3d pProjectPoint = ProjectPointsToLine(pt, cLineParam);

    return Euler2DDistance(pt, pProjectPoint);
}

// 得到点到直线的投影点
cv::Point3d CVLImgProcBase::ProjectPointsToLine(cv::Point3d pt, S_2D_LINE cLineParam)
{
    int nIdx = 0;
    int sIdx = 0;
    cv::Point3d pProjectPoint;
    if (1 == cLineParam._line_type)
    {
        // y = k1 * x + b1;
        // x = k2 * y + b2;

        // k2 = -k1;
        // b2 = x1 - k2 * y1;
        // (x1, y1) is the real point of the connected component
        S_2D_LINE cVertLineParam;
        cVertLineParam._k = cLineParam._k * (-1.0);
        cVertLineParam._b = pt.x - (cVertLineParam._k * pt.y);

        // y = ((k1 * b2) + b1) / (1 - k1 * k2);
        // x = k2 * y + b2;
        pProjectPoint.y = (cLineParam._k * cVertLineParam._b +
            cLineParam._b) /
            (1.0 - (cLineParam._k * cVertLineParam._k));
        pProjectPoint.x = cVertLineParam._k * pProjectPoint.y + cVertLineParam._b;
    }
    else if (0 == cLineParam._line_type)
    {
        // x = k1 * y + b1;
        // y = k2 * x + b2;

        // k2 = -k1;
        // b2 = y1 - k2 * x1;
        // (x1, y1) is the real point of the connected component
        S_2D_LINE cHoriLineParam;
        cHoriLineParam._k = cLineParam._k * (-1.0);
        cHoriLineParam._b = pt.y - (cHoriLineParam._k * pt.x);

        // y = ((k2 * b1) + b2) / (1 - k1 * k2);
        // x = k1 * y + b1;
        pProjectPoint.y = (cHoriLineParam._k * cLineParam._b + cHoriLineParam._b) /
            (1.0 - (cLineParam._k * cHoriLineParam._k));
        pProjectPoint.x = cLineParam._k * pProjectPoint.y + cLineParam._b;
    }

    return pProjectPoint;
}

// 得到直线方程
S_2D_LINE CVLImgProcBase::GetLineParam(cv::Point3d pt1, cv::Point3d pt2)
{
    S_2D_LINE cLineParam;
    if (fabs(pt2.y - pt1.y) > fabs(pt2.x - pt1.x))
    {
        // x = k * y + b;
        cLineParam._k = (pt2.x - pt1.x) / (pt2.y - pt1.y+0.0001);
        cLineParam._b = pt1.x - (cLineParam._k * pt1.y);
        cLineParam._line_type = 0;
    }
    else
    {
        // y = k * x + b;
        cLineParam._k = (pt2.y - pt1.y) / (pt2.x - pt1.x + 0.0001);
        cLineParam._b = pt1.y - (cLineParam._k * pt1.x);
        cLineParam._line_type = 1;
    }

    return cLineParam;
}

// 二维平面上两点的欧式距离
double CVLImgProcBase::Euler2DDistance(cv::Point3d pt1, cv::Point3d pt2)
{
    return sqrt(pow(pt1.x - pt2.x, 2.0) + pow(pt1.y - pt2.y, 2.0));
}
