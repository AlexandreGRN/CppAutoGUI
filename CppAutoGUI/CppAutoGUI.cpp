#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>
#include <string>
#include <typeinfo>
#include <opencv2/highgui.hpp>
#include <chrono>
#include <thread>

using namespace cv;
using namespace std;

struct screenStruct {
    Mat screenInfo;
    int screenWidth;
    int screenHeight;
} SCREENSHOT;

struct imageStruct {
    Mat image;
    int r1;
    int g1;
    int b1;
    int imageWidth;
    int rowsHeight;
} IMG;

struct match {
    bool isMatch;
    int matchStartI;
    int matchStartJ;
    int matchEndI;
    int matchEndJ;
} MATCH;

BITMAPINFOHEADER createBitmapHeader(int width, int height)
{
    BITMAPINFOHEADER  bi;

    // create a bitmap
    bi.biSize = sizeof(BITMAPINFOHEADER);
    bi.biWidth = width;
    bi.biHeight = -height;  //this is the line that makes it draw upside down or not
    bi.biPlanes = 1;
    bi.biBitCount = 32;
    bi.biCompression = BI_RGB;
    bi.biSizeImage = 0;
    bi.biXPelsPerMeter = 0;
    bi.biYPelsPerMeter = 0;
    bi.biClrUsed = 0;
    bi.biClrImportant = 0;

    return bi;
}

screenStruct captureScreenMat(HWND hwnd)
{
    Mat src;

    // get handles to a device context (DC)
    HDC hwindowDC = GetDC(hwnd);
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    // define scale, height and width
    int screenx = 0;
    int screeny = 0;
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    // create mat object
    src.create(height, width, CV_8UC4);

    // create a bitmap
    HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    BITMAPINFOHEADER bi = createBitmapHeader(width, height);

    // use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);

    // copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);  //change SRCCOPY to NOTSRCCOPY for wacky colors !
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);            //copy from hwindowCompatibleDC to hbwindow

    // avoid memory leak
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return { src, width, height };
}

imageStruct captureImageMat(string imgUrl) {
    Mat image = imread("C:/Users/tulki/Documents/Projects/Waven/img/Sanstitre.png");
    int imgr1 = image.at<Vec3b>(0, 0)[2];
    int imgg1 = image.at<Vec3b>(0, 0)[1];
    int imgb1 = image.at<Vec3b>(0, 0)[0];
    int imgcols = image.cols;
    int imgrows = image.rows;

    cv::imwrite("C:\\Users\\tulki\\Documents\\Screenshot2.png", image);
    return { image, imgr1, imgg1, imgb1, imgcols, imgrows };
}

bool checkForCompleteMatch(screenStruct screen, imageStruct img, int screenI, int screenJ) {
    for (int i = 0; i < img.image.rows - 1; i++) {
        for (int j = 0; j < img.image.cols - 1; j++) {
            int rscreen = screen.screenInfo.data[screen.screenInfo.channels() * (screen.screenInfo.cols * (screenI + i) + (screenJ + j)) + 2];
            int gscreen = screen.screenInfo.data[screen.screenInfo.channels() * (screen.screenInfo.cols * (screenI + i) + (screenJ + j)) + 1];
            int bscreen = screen.screenInfo.data[screen.screenInfo.channels() * (screen.screenInfo.cols * (screenI + i) + (screenJ + j)) + 0];
            int rimage = img.image.data[img.image.channels() * (img.image.cols * i + j) + 2];
            int gimage = img.image.data[img.image.channels() * (img.image.cols * i + j) + 1];
            int bimage = img.image.data[img.image.channels() * (img.image.cols * i + j) + 0];
            if (rscreen != rimage || gscreen != gimage || bscreen != bimage) { return false; }
        }
    }
    cout << "true\n";
    SetCursorPos(screenJ, screenI);
    return true;
}

int findFirstMatchingPixelOnScreen(screenStruct screen, imageStruct img) {
    for (int i = 0; i < screen.screenHeight; i++) {
        for (int j = 0; j < screen.screenWidth; j++) {
            int r = screen.screenInfo.data[screen.screenInfo.channels() * (screen.screenInfo.cols * i + j) + 2];
            int g = screen.screenInfo.data[screen.screenInfo.channels() * (screen.screenInfo.cols * i + j) + 1];
            int b = screen.screenInfo.data[screen.screenInfo.channels() * (screen.screenInfo.cols * i + j) + 0];

            if (r == img.r1 && g == img.g1 && b == img.b1) {
                cout << "match\n";
                checkForCompleteMatch(screen, img, i, j);
            }
        }
    }
    return 1;
}

int main()
{
    // capture image
    HWND hwnd = GetDesktopWindow();
    screenStruct src = captureScreenMat(hwnd);
    imageStruct img = captureImageMat("a");

    findFirstMatchingPixelOnScreen(src, img);
    // save img
    cv::imwrite("C:\\Users\\tulki\\Documents\\Screenshot.png", src.screenInfo);


    return 0;
}