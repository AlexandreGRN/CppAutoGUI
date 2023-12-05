
#include "ImageSource.h"
#include "Libraries.h"

ImageSource::ImageSource(std::string imgPath) : Image() {
    captureImageMat(imgPath);
}

/*
    This function create the struct containing the image object and all the necessary infos related to it

    IN: -------------------------------------------------------------
        hwnd: the path to the object

    OUT: ------------------------------------------------------------
        all the infos about the image:{
            the image
            RGB values
            its width
            its height
        }
*/
void ImageSource::captureImageMat(std::string imgPath) {
    // Stock the image in a variable
    cv::Mat image = cv::imread(imgPath, cv::IMREAD_UNCHANGED);

    // Get all the datas
    int imgcols = image.cols;
    int imgrows = image.rows;
    int imgr1, imgg1, imgb1, imgalpha1, firstColorCoordinateI, firstColorCoordinateJ;
    for (int i = 0; i < image.rows; i++) {
        for (int j = 0; j < image.cols; j++) {
            if (int(image.at<cv::Vec4b>(i, j)[3]) != 0) {
                imgr1 = image.at<cv::Vec4b>(i, j)[2];
                imgg1 = image.at<cv::Vec4b>(i, j)[1];
                imgb1 = image.at<cv::Vec4b>(i, j)[0];
                imgalpha1 = image.at<cv::Vec4b>(i, j)[3];
                firstColorCoordinateI = i;
                firstColorCoordinateJ = j;
                goto found;
            }
        }
    }
    throw 99;

found:
    this->b1 = imgb1;
    this->g1 = imgg1;
    this->r1 = imgr1;
    this->imageWidth = imgcols;
    this->imageHeight = imgrows;
    this->firstI = firstColorCoordinateI;
    this->firstJ = firstColorCoordinateJ;
    this->image = image;
}