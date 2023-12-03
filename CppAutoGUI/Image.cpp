
#include "Image.h"
#include "Libraries.h"

// Setters and getters
cv::Mat Image::getImage()
{
    return image;
};
void Image::setImage(cv::Mat Image)
{
    image = Image;
}
int Image::getR1()
{
    return r1;
};
void Image::setR1(int r)
{
    r1 = r;
}
int Image::getG1()
{
    return g1;
};
void Image::setG1(int g)
{
	g1 = g;
}
int Image::getB1()
{
    return b1;
}
void Image::setB1(int b)
{
	b1 = b;
}
int Image::getImageWidth()
{
    return imageWidth;
};
void Image::setImageWidth(int width)
{
	imageWidth = width;
}
int Image::getImageHeight()
{
    return imageHeight;
}
void Image::setImageHeight(int height)
{
	imageHeight = height;
}
int Image::getFirstICoordinate()
{
    return firstI;
}
void Image::setFirstICoordinate(int i)
{
	firstI = i;
}
int Image::getFirstJCoordinate()
{
    return firstJ;
}
void Image::setFirstJCoordinate(int j)
{
	firstJ = j;
}