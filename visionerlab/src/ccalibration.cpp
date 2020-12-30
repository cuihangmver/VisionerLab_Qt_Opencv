#include "ccalibration.h"

CCalibration::CCalibration()
{

}

void CCalibration::StructedLightReadData
(
        std::string sPath,
        std::vector<cv::Point> &vpLeft,
        std::vector<cv::Point> &vpRight
)
{
    std::ifstream inData(sPath);
    while(inData)
    {
        cv::Point pPointLeft, pPointRight;
        inData >> pPointLeft.x >> pPointLeft.y >> pPointRight.x >> pPointRight.y;
        if(0 == pPointLeft.x)
            continue;
        vpLeft.push_back(pPointLeft);
        vpRight.push_back(pPointRight);
    }
}

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
)
{
    // 计算焦距
    std::vector<double> vdCamDis;
    dF = 0;
    for(int i = 0; i < vpLeft.size(); i++)
    {
        double dCamDis = fabs(vpRight[i].x-vpLeft[i].x)*dC;
        if(0 == i)
        {
            vdCamDis.push_back(dCamDis);
            continue;
        }
        vdCamDis.push_back(dCamDis);
        double dA = vdCamDis[i-1] * vdCamDis[i] * dPerMove;
        double dB = dDis2Line * fabs(vdCamDis[i-1] - vdCamDis[i]);
        dF += (dA / dB);

    }
    dF /= (vpLeft.size()-1);

    // 计算激光线在图像中心时高度
    dCenterDis = 0;
    std::vector<double> vdCenterDis;
    for(int i = 0; i < vdCamDis.size(); i++)
    {
        vdCenterDis.push_back((dDis2Line * dF / vdCamDis[i]));
        dCenterDis += (dDis2Line * dF / vdCamDis[i]);
    }
    dCenterDis /= vdCamDis.size();

    // 计算激光夹角cotθ
    std::vector<double> vdYPiexl;
    std::vector<double> vdYGap;
    for(int i = 0; i < vdCenterDis.size(); i++)
    {
        double dYPiexl = (double)(vpLeft[i].y + vpRight[i].y) / 2.0;
        double dY = vdCenterDis[i] * (dYPiexl - pWidthHeight.y / 2) / (dF / dC);
        if(0 == i)
        {
            vdYPiexl.push_back(dY);
            continue;
        }
        vdYPiexl.push_back(dY);
        vdYGap.push_back(fabs(vdYPiexl[i]-vdYPiexl[i-1]));
    }
    dcot = 0;
    for(int i = 0; i < vdYGap.size(); i++)
    {
        dcot += vdYGap[i] / dPerMove;
    }
    dcot = dcot / vdYGap.size();

    // 计算基线长
    dBaseLine = dCenterDis * dcot;
}
