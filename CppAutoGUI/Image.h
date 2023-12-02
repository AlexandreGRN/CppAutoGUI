#pragma once

#include "Libraries.h"
class Image
{
public:
    cv::Mat image;
    int r1 = 0;
    int g1 = 0;
    int b1 = 0;
    int imageWidth = 0;
    int imageHeight = 0;
    int firstI = 0;
    int firstJ = 0;
    bool bIsScreen = false;
};