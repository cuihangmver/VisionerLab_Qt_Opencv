//*************************************
/*
 * 数据结构，包括结构体、类。
                                     */
//*************************************
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
    typedef struct S_2D_LINE
    {
        double _k;
        double _b;
        int _line_type;   // 横：1，竖：0
    }S_2D_LINE;

    class InforBase
    {
    public:
        InforBase();
    };
}
#endif // INFORBASE_H
