#pragma once

#include "Libraries.h"
class Image
{
// Properties --------------------------------------------------------
private:
    cv::Mat image;
    int r1 = 0;
    int g1 = 0;
    int b1 = 0;
    int imageWidth = 0;
    int imageHeight = 0;
    int firstI = 0;
    int firstJ = 0;

public:
    bool bIsScreen = false;

// Methods -----------------------------------------------------------
public:
    // Getters
    cv::Mat getImage();
	int getR1();
	int getG1();
	int getB1();
	int getImageWidth();
	int getImageHeight();
	int getFirstICoordinate();
	int getFirstJCoordinate();

    // Setters
	void setImage(cv::Mat Image);
    void setR1(int r);
    void setG1(int g);
    void setB1(int b);
    void setImageWidth(int width);
    void setImageHeight(int height);
    void setFirstICoordinate(int i);
    void setFirstJCoordinate(int j);
};