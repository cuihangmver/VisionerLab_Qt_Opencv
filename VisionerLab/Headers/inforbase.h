#ifndef INFORBASE_H
#define INFORBASE_H
#include <iostream>

namespace INFOR_BASE
{
    typedef struct sImgInfor
    {
        int nWidth;
        int nHeight;
        int nDepth;
        std::string sPath;
        // 图像大小
        double dSize;
        // png、jpg...
        std::string sFormat;
        // 灰度、彩色、二值图...
        std::string sColorSpace;
        // 透明度
        std::string sAlpha;
    }sImgInfor;
    class InforBase
    {
    public:
        InforBase();
    };
}
#endif // INFORBASE_H
