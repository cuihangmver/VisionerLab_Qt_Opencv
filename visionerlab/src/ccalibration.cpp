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
//--------------- 结构光标定 --------------------//

//**************** 手眼标定（眼在手上） ******************//

void CCalibration::HandEyeReadData
(
        std::string sPathEyeHand,
        std::string sPathTCP,
        std::vector<Eigen::MatrixXd> &vmEyeHand,
        std::vector<Eigen::MatrixXd> &vmTCP
)
{
    std::ifstream inEyeHandMatrix(sPathEyeHand);
    std::ifstream inTCPMatrix(sPathTCP);
    QString a = QString::fromStdString(sPathEyeHand);
    QString b = QString::fromStdString(sPathTCP);
    qDebug() << a;
    qDebug() << b;
    double dX, dY, dZ, dRX, dRY, dRZ;
    while(inEyeHandMatrix)
    {
        inEyeHandMatrix >> dX >> dY >> dZ >> dRX >> dRY >> dRZ;
        Eigen::MatrixXd mEyeHandMatrix4(4, 4);
        Eigen::Matrix3d mEyeHandMatrix3;
        Eigen::Vector3d vEuler(dRX, dRY, dRZ);

        // 欧拉角转旋转矩阵
        mEyeHandMatrix3 = Eigen::AngleAxisd(vEuler[0], Eigen::Vector3d::UnitX()) *
                          Eigen::AngleAxisd(vEuler[1], Eigen::Vector3d::UnitY()) *
                          Eigen::AngleAxisd(vEuler[2], Eigen::Vector3d::UnitZ());
        mEyeHandMatrix4(0, 0) = mEyeHandMatrix3(0, 0);
        mEyeHandMatrix4(0, 1) = mEyeHandMatrix3(0, 1);
        mEyeHandMatrix4(0, 2) = mEyeHandMatrix3(0, 2);
        mEyeHandMatrix4(0, 3) = dX;
        mEyeHandMatrix4(1, 0) = mEyeHandMatrix3(1, 0);
        mEyeHandMatrix4(1, 1) = mEyeHandMatrix3(1, 1);
        mEyeHandMatrix4(1, 2) = mEyeHandMatrix3(1, 2);
        mEyeHandMatrix4(1, 3) = dY;
        mEyeHandMatrix4(2, 0) = mEyeHandMatrix3(2, 0);
        mEyeHandMatrix4(2, 1) = mEyeHandMatrix3(2, 1);
        mEyeHandMatrix4(2, 2) = mEyeHandMatrix3(2, 2);
        mEyeHandMatrix4(2, 3) = dZ;
        mEyeHandMatrix4(3, 0) = 0;
        mEyeHandMatrix4(3, 1) = 0;
        mEyeHandMatrix4(3, 2) = 0;
        mEyeHandMatrix4(3, 3) = 1;
        qDebug() << "InputCam : ";
        qDebug() << mEyeHandMatrix4(0, 0) << " " << mEyeHandMatrix4(0, 1) << " "<< mEyeHandMatrix4(0, 2) << " "<< mEyeHandMatrix4(0, 3);
        qDebug() << mEyeHandMatrix4(1, 0) << " " << mEyeHandMatrix4(1, 1) << " "<< mEyeHandMatrix4(1, 2) << " "<< mEyeHandMatrix4(1, 3);
        qDebug() << mEyeHandMatrix4(2, 0) << " " << mEyeHandMatrix4(2, 1) << " "<< mEyeHandMatrix4(2, 2) << " "<< mEyeHandMatrix4(2, 3);
        qDebug() << mEyeHandMatrix4(3, 0) << " " << mEyeHandMatrix4(3, 1) << " "<< mEyeHandMatrix4(3, 2) << " "<< mEyeHandMatrix4(3, 3);
        vmEyeHand.push_back(mEyeHandMatrix4);
    }

    while(inTCPMatrix)
    {
        inTCPMatrix >> dX >> dY >> dZ >> dRX >> dRY >> dRZ;
        Eigen::MatrixXd mTCPMatrix4(4, 4);
        Eigen::Matrix3d mTCPMatrix3;
        Eigen::Vector3d vEuler(dRX, dRY, dRZ);

        // 欧拉角转旋转矩阵
        mTCPMatrix3 = Eigen::AngleAxisd(vEuler[0], Eigen::Vector3d::UnitX()) *
                      Eigen::AngleAxisd(vEuler[1], Eigen::Vector3d::UnitY()) *
                      Eigen::AngleAxisd(vEuler[2], Eigen::Vector3d::UnitZ());
        mTCPMatrix4(0, 0) = mTCPMatrix3(0, 0);
        mTCPMatrix4(0, 1) = mTCPMatrix3(0, 1);
        mTCPMatrix4(0, 2) = mTCPMatrix3(0, 2);
        mTCPMatrix4(0, 3) = dX;
        mTCPMatrix4(1, 0) = mTCPMatrix3(1, 0);
        mTCPMatrix4(1, 1) = mTCPMatrix3(1, 1);
        mTCPMatrix4(1, 2) = mTCPMatrix3(1, 2);
        mTCPMatrix4(1, 3) = dY;
        mTCPMatrix4(2, 0) = mTCPMatrix3(2, 0);
        mTCPMatrix4(2, 1) = mTCPMatrix3(2, 1);
        mTCPMatrix4(2, 2) = mTCPMatrix3(2, 2);
        mTCPMatrix4(2, 3) = dZ;
        mTCPMatrix4(3, 0) = 0;
        mTCPMatrix4(3, 1) = 0;
        mTCPMatrix4(3, 2) = 0;
        mTCPMatrix4(3, 3) = 1;
        qDebug() << "InputTCP : ";
        qDebug() << mTCPMatrix4(0, 0) << " " << mTCPMatrix4(0, 1) << " "<< mTCPMatrix4(0, 2) << " "<< mTCPMatrix4(0, 3);
        qDebug() << mTCPMatrix4(1, 0) << " " << mTCPMatrix4(1, 1) << " "<< mTCPMatrix4(1, 2) << " "<< mTCPMatrix4(1, 3);
        qDebug() << mTCPMatrix4(2, 0) << " " << mTCPMatrix4(2, 1) << " "<< mTCPMatrix4(2, 2) << " "<< mTCPMatrix4(2, 3);
        qDebug() << mTCPMatrix4(3, 0) << " " << mTCPMatrix4(3, 1) << " "<< mTCPMatrix4(3, 2) << " "<< mTCPMatrix4(3, 3);
        vmTCP.push_back(mTCPMatrix4);
    }
}
/*
Eigen::MatrixXd CCalibration::HandEyeCalibration
(
        std::vector<Eigen::MatrixXd> vmCam,
        std::vector<Eigen::MatrixXd> vmTCP
)
{
    Eigen::Matrix3d mCamMatrix3;
    Eigen::Matrix3d mTCPMatrix3;
    Eigen::MatrixXd mK(9*vmCam.size(), 9);
    std::vector<Eigen::MatrixXd> vmK;
    Eigen::Matrix3d mI = Eigen::Matrix3d::Identity();
    // RA*RX=RX*RB
    for(int i = 0; i < vmCam.size(); i++)
    {
        Eigen::Matrix3d mCamMatrix3;
        mCamMatrix3(0, 0) = vmCam[i](0, 0);
        mCamMatrix3(0, 1) = vmCam[i](0, 1);
        mCamMatrix3(0, 2) = vmCam[i](0, 2);
        mCamMatrix3(1, 0) = vmCam[i](1, 0);
        mCamMatrix3(1, 1) = vmCam[i](1, 1);
        mCamMatrix3(1, 2) = vmCam[i](1, 2);
        mCamMatrix3(2, 0) = vmCam[i](2, 0);
        mCamMatrix3(2, 1) = vmCam[i](2, 1);
        mCamMatrix3(2, 2) = vmCam[i](2, 2);
        Eigen::Matrix3d mTCPMatrix3;
        mTCPMatrix3(0, 0) = vmTCP[i](0, 0);
        mTCPMatrix3(0, 1) = vmTCP[i](0, 1);
        mTCPMatrix3(0, 2) = vmTCP[i](0, 2);
        mTCPMatrix3(1, 0) = vmTCP[i](1, 0);
        mTCPMatrix3(1, 1) = vmTCP[i](1, 1);
        mTCPMatrix3(1, 2) = vmTCP[i](1, 2);
        mTCPMatrix3(2, 0) = vmTCP[i](2, 0);
        mTCPMatrix3(2, 1) = vmTCP[i](2, 1);
        mTCPMatrix3(2, 2) = vmTCP[i](2, 2);
        Eigen::Matrix3d mA = mTCPMatrix3.inverse() * mTCPMatrix3;
        Eigen::Matrix3d mB = mCamMatrix3.inverse() * mCamMatrix3;

        Eigen::MatrixXd mIABI(9, 9);

        mIABI = Eigen::kroneckerProduct(mI, mA) - Eigen::kroneckerProduct(mA, mI);
        vmK.push_back(mIABI);
    }
    mK << vmK[0],
          vmK[1],
          vmK[2],
          vmK[3],
          vmK[4],
          vmK[5],
          vmK[6],
          vmK[7],
          vmK[8],
          vmK[9],
          vmK[10],
          vmK[11],
          vmK[12],
          vmK[13],
          vmK[14];
    Eigen::JacobiSVD<Eigen::MatrixXd> svd1(mK, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::MatrixXd U1 = svd1.matrixU();
    Eigen::MatrixXd V1 = svd1.matrixV();
    Eigen::MatrixXd A1 = svd1.singularValues();
    Eigen::MatrixXd V1T = V1.transpose();

    Eigen::Matrix3d mRTem;
    mRTem(0, 0) = V1T(V1T.rows()-1, 0);
    mRTem(0, 1) = V1T(V1T.rows()-1, 1);
    mRTem(0, 2) = V1T(V1T.rows()-1, 2);
    mRTem(1, 0) = V1T(V1T.rows()-1, 3);
    mRTem(1, 1) = V1T(V1T.rows()-1, 4);
    mRTem(1, 2) = V1T(V1T.rows()-1, 5);
    mRTem(2, 0) = V1T(V1T.rows()-1, 6);
    mRTem(2, 1) = V1T(V1T.rows()-1, 7);
    mRTem(2, 2) = V1T(V1T.rows()-1, 8);

    Eigen::JacobiSVD<Eigen::MatrixXd> svd2(mK, Eigen::ComputeThinU | Eigen::ComputeThinV);
    Eigen::MatrixXd U2 = svd2.matrixU();
    Eigen::MatrixXd V2 = svd2.matrixV();
    Eigen::MatrixXd A2 = svd2.singularValues();
    Eigen::MatrixXd V2T = V2.transpose();

    Eigen::Matrix3d mR = U2 * V2T;

    // RA*tx+tA = RX*tB+tX
    // (RA-I)tx = RX*tB-tA
    // A * tx = B
    Eigen::MatrixXd A(3*vmCam.size(), 3);
    Eigen::MatrixXd B(3*vmCam.size(), 1);
    std::vector<Eigen::Matrix3d> vA;
    std::vector<Eigen::Vector3d> vB;
    for(int i = 0; i < vmCam.size(); i++)
    {
        Eigen::Matrix3d mCamMatrix3;
        mCamMatrix3(0, 0) = vmCam[i](0, 0);
        mCamMatrix3(0, 1) = vmCam[i](0, 1);
        mCamMatrix3(0, 2) = vmCam[i](0, 2);
        mCamMatrix3(1, 0) = vmCam[i](1, 0);
        mCamMatrix3(1, 1) = vmCam[i](1, 1);
        mCamMatrix3(1, 2) = vmCam[i](1, 2);
        mCamMatrix3(2, 0) = vmCam[i](2, 0);
        mCamMatrix3(2, 1) = vmCam[i](2, 1);
        mCamMatrix3(2, 2) = vmCam[i](2, 2);
        Eigen::Vector3d vVectorA;
        vVectorA(0) = vmCam[i](0, 3);
        vVectorA(1) = vmCam[i](1, 3);
        vVectorA(2) = vmCam[i](2, 3);

        Eigen::Matrix3d mA = mTCPMatrix3.inverse() * mTCPMatrix3 - mI;
        Eigen::Matrix3d mB = mCamMatrix3.inverse() * mCamMatrix3;
        vA.push_back(mA);

    }
    //Eigen::Matrix3d A =
    return A;
}
*/

Eigen::Matrix4d CCalibration::HandEyeCalibration
(
        std::vector<Eigen::MatrixXd> vmCam,
        std::vector<Eigen::MatrixXd> vmTCP
)
{
    int nNum = 6;
    // 定义RA，RB。（RA是TCP，RB是Cam）
    Eigen::Matrix3d mCamMatrix3;
    Eigen::Matrix3d mTCPMatrix3;
    Eigen::MatrixXd mK(9*nNum, 9);
    std::vector<Eigen::MatrixXd> vmK;

    std::vector<Eigen::Matrix3d> vRA;
    std::vector<Eigen::Matrix3d> vRB;
    std::vector<Eigen::Vector3d> vTA;
    std::vector<Eigen::Vector3d> vTB;
    Eigen::Matrix3d mI = Eigen::Matrix3d::Identity();

    // RA*RX=RX*RB
    for(int i = 0; i < vmCam.size() - 1; i++)
    {
        Eigen::Matrix4d mA = vmTCP[i].inverse() * vmTCP[i+1];
        Eigen::Matrix4d mB = vmCam[i+1] * vmCam[i].inverse();
        Eigen::Matrix3d mRA;
        Eigen::Vector3d mTA;
        mRA(0, 0) = mA(0, 0);
        mRA(0, 1) = mA(0, 1);
        mRA(0, 2) = mA(0, 2);
        mRA(1, 0) = mA(1, 0);
        mRA(1, 1) = mA(1, 1);
        mRA(1, 2) = mA(1, 2);
        mRA(2, 0) = mA(2, 0);
        mRA(2, 1) = mA(2, 1);
        mRA(2, 2) = mA(2, 2);
        mTA(0) = mA(0, 3);
        mTA(1) = mA(1, 3);
        mTA(2) = mA(2, 3);
        Eigen::Matrix3d mRB;
        Eigen::Vector3d mTB;
        mRB(0, 0) = mB(0, 0);
        mRB(0, 1) = mB(0, 1);
        mRB(0, 2) = mB(0, 2);
        mRB(1, 0) = mB(1, 0);
        mRB(1, 1) = mB(1, 1);
        mRB(1, 2) = mB(1, 2);
        mRB(2, 0) = mB(2, 0);
        mRB(2, 1) = mB(2, 1);
        mRB(2, 2) = mB(2, 2);
        mTB(0) = mB(0, 3);
        mTB(1) = mB(1, 3);
        mTB(2) = mB(2, 3);
        vRA.push_back(mRA);
        vRB.push_back(mRB);
        vTA.push_back(mTA);
        vTB.push_back(mTB);

        Eigen::MatrixXd mIABI(9, 9);
        mIABI = Eigen::kroneckerProduct(mI, mRA) - Eigen::kroneckerProduct(mRB, mI);
        vmK.push_back(mIABI);
    }
    mK << vmK[0],
          vmK[1],
          vmK[2],
          vmK[3],
          vmK[4],
          vmK[5];
    /*
          vmK[6],
          vmK[7],
          vmK[8],
          vmK[9],
          vmK[10],
          vmK[11],
          vmK[12],
          vmK[13],
          vmK[14];
          */
    std::ofstream outK("k.txt");
    for(int i = 0; i < mK.rows(); i++)
    {
        for(int j = 0; j < mK.cols(); j++)
        {
            outK << mK(i, j) << " ";
        }
        outK << std::endl;
    }
    Eigen::JacobiSVD<Eigen::MatrixXd> svd1(mK, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::MatrixXd U1 = svd1.matrixU();
    Eigen::MatrixXd V1 = svd1.matrixV();
    Eigen::MatrixXd A1 = svd1.singularValues();
    Eigen::MatrixXd V1T = V1;
    Eigen::Matrix3d mRXTem;
    qDebug() << "V1T.rows() = " << V1T.rows();
    qDebug() << "V1T.cols() = " << V1T.cols();
    mRXTem(0, 0) = V1T(V1T.rows()-1, 0);
    mRXTem(0, 1) = V1T(V1T.rows()-1, 1);
    mRXTem(0, 2) = V1T(V1T.rows()-1, 2);
    mRXTem(1, 0) = V1T(V1T.rows()-1, 3);
    mRXTem(1, 1) = V1T(V1T.rows()-1, 4);
    mRXTem(1, 2) = V1T(V1T.rows()-1, 5);
    mRXTem(2, 0) = V1T(V1T.rows()-1, 6);
    mRXTem(2, 1) = V1T(V1T.rows()-1, 7);
    mRXTem(2, 2) = V1T(V1T.rows()-1, 8);
    std::ofstream outA1("outA1.txt");
    for(int i = 0; i < A1.rows(); i++)
    {
        for(int j = 0; j < A1.cols(); j++)
        {
            outA1 << A1(i, j) << " ";
        }
        outA1 << std::endl;
    }
    std::ofstream outV1T("outV1T.txt");
    for(int i = 0; i < V1T.rows(); i++)
    {
        for(int j = 0; j < V1T.cols(); j++)
        {
            outV1T << V1T(i, j) << " ";
        }
        outV1T << std::endl;
    }
    std::ofstream outmRXTem("outmRXTem.txt");
    for(int i = 0; i < mRXTem.rows(); i++)
    {
        for(int j = 0; j < mRXTem.cols(); j++)
        {
            outmRXTem << mRXTem(i, j) << " ";
        }
        outmRXTem << std::endl;
    }
    Eigen::JacobiSVD<Eigen::MatrixXd> svd2(mRXTem, Eigen::ComputeFullU | Eigen::ComputeFullV);
    Eigen::MatrixXd U2 = svd2.matrixU();
    Eigen::MatrixXd V2 = svd2.matrixV();
    Eigen::MatrixXd A2 = svd2.singularValues();
    Eigen::MatrixXd V2T = V2.transpose();
    qDebug() << "U2.rows() = " << U2.rows();
    qDebug() << "U2.cols() = " << U2.cols();
    qDebug() << "V2T.rows() = " << V2T.rows();
    qDebug() << "V2T.cols() = " << V2T.cols();
    // 得到旋转矩阵
    Eigen::Matrix3d mRX = U2 * V2T;
    std::ofstream outmRX("outmRX.txt");
    for(int i = 0; i < mRX.rows(); i++)
    {
        for(int j = 0; j < mRX.cols(); j++)
        {
            outmRX << mRX(i, j) << " ";
        }
        outmRX << std::endl;
    }
    // RA*tx+tA = RX*tB+tx
    // (RA-I)tx = RX*tB-tA
    // A * tx = B
    Eigen::MatrixXd xA(3*nNum, 3);
    Eigen::MatrixXd xB(3*nNum, 1);
    std::vector<Eigen::Matrix3d> vA;
    std::vector<Eigen::MatrixXd> vB;
    for(int i = 0; i < nNum; i++)
    {
        Eigen::Matrix3d A = vRA[i] - mI;
        Eigen::MatrixXd TATem(3, 1);
        TATem(0,0) = vTA[i](0);
        TATem(1,0) = vTA[i](1);
        TATem(2,0) = vTA[i](2);
        Eigen::MatrixXd TBTem(3, 1);
        TBTem(0,0) = vTB[i](0);
        TBTem(1,0) = vTB[i](1);
        TBTem(2,0) = vTB[i](2);
        Eigen::MatrixXd B(3, 1);
        B = mRX*TBTem - TATem;
        vA.push_back(A);
        vB.push_back(B);
    }
    xA << vA[0],
            vA[1],
            vA[2],
            vA[3],
            vA[4],
            vA[5];
            /*
            vA[6],
            vA[7],
            vA[8],
            vA[9],
            vA[10],
            vA[11],
            vA[12],
            vA[13];
            vA[14];
                    */
    xB << vB[0],
            vB[1],
            vB[2],
            vB[3],
            vB[4],
            vB[5];
    /*
            vB[6],
            vB[7],
            vB[8],
            vB[9],
            vB[10],
            vB[11],
            vB[12],
            vB[13];
            vB[14];
            */
    // 最小二乘解 (ATA)^(-1)*AT*B
    Eigen::Matrix3d mATA = xA.transpose() * xA;
    Eigen::MatrixXd mTX(3, 1);
    mTX = mATA.inverse() * xA.transpose() * xB;
    qDebug() << "5";
    Eigen::Matrix4d RT;
    RT(0, 0) = mRX(0, 0);
    RT(0, 1) = mRX(0, 1);
    RT(0, 2) = mRX(0, 2);
    RT(0, 3) = mTX(0, 0);
    RT(1, 0) = mRX(1, 0);
    RT(1, 1) = mRX(1, 1);
    RT(1, 2) = mRX(1, 2);
    RT(1, 3) = mTX(1, 0);
    RT(2, 0) = mRX(2, 0);
    RT(2, 1) = mRX(2, 1);
    RT(2, 2) = mRX(2, 2);
    RT(2, 3) = mTX(2, 0);
    RT(3, 0) = 0;
    RT(3, 1) = 0;
    RT(3, 2) = 0;
    RT(3, 3) = 1;
    qDebug() << "RT";
    qDebug() << RT(0, 0);
    qDebug() << RT(0, 1);
    qDebug() << RT(0, 2);
    qDebug() << RT(0, 3);
    qDebug() << RT(1, 0);
    qDebug() << RT(1, 1);
    qDebug() << RT(1, 2);
    qDebug() << RT(1, 3);
    qDebug() << RT(2, 0);
    qDebug() << RT(2, 1);
    qDebug() << RT(2, 2);
    qDebug() << RT(2, 3);
    qDebug() << RT(3, 0);
    qDebug() << RT(3, 1);
    qDebug() << RT(3, 2);
    qDebug() << RT(3, 3);
    return RT;
}
//---------------- 手眼标定（眼在手上） ------------------//
