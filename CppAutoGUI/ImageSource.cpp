
#include "ImageSource.h"

autoGUI::ImageSource::ImageSource(std::string const imgPath) : Image() {
    captureImageMat(imgPath);
}

autoGUI::ImageSource::~ImageSource() {
	this->image.release();
}

autoGUI::ImageSource::ImageSource(ImageSource&& Copy) noexcept : Image(Copy.r1, Copy.g1, Copy.b1, Copy.imageWidth, Copy.imageHeight, Copy.firstI, Copy.firstJ) {
    this->image = std::move(Copy.image);
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
void autoGUI::ImageSource::captureImageMat(std::string const imgPath) {
    // Stock the image in a variable
    cv::Mat image = cv::imread(imgPath, cv::IMREAD_UNCHANGED);

    // Get all the datas
    int imgcols = image.cols;
    int imgrows = image.rows;
    int imgr1, imgg1, imgb1, imgalpha1, firstColorCoordinateI, firstColorCoordinateJ;
    for (int i = 0; i < imgrows; i++) {
        for (int j = 0; j < imgcols; j++) {
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