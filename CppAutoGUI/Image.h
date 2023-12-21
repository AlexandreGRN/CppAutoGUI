#pragma once

#include "Libraries.h"

// Image abstract class to be inherited by Screenshot and ImageSource
class Image
{
// Properties --------------------------------------------------------
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

    Image() = default;
    Image(int r1, int g1, int b1, int imageWidth, int imageHeight, int firstI, int firstJ) : r1(r1), g1(g1), b1(b1), imageWidth(imageWidth), imageHeight(imageHeight), firstI(firstI), firstJ(firstJ) {}
};