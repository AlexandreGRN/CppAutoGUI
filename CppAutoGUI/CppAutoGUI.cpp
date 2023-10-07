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

struct coordinate2D {
    int x1;
    int y1;
    int x2;
    int y2;
    int xMiddle;
    int yMiddle;
};

/*
    This function create a Bitmap Header containing all the information to create the BitMap of an image

    IN: -------------------------------------------------------------
        width: The width of the image (1980 for a screenshot)
        height: The height of the screen (1080 for a screenshot)

    OUT: ------------------------------------------------------------
        The BitMap Header of the size of an image


    What is a BitMap:
        A Bitmap is a representation of an image, here, A bitMap Header contains all the necessary informations for the creation of a bitMap
*/
BITMAPINFOHEADER createBitmapHeader(int width, int height){
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

/*
    This function create the struct containing the screenshot object and all the necessary infos related to it

    IN: -------------------------------------------------------------
        hwnd: the desktop window we want to capture

    OUT: ------------------------------------------------------------
        all the infos about the screenshot:{
            the screenshot
            the screen width
            the screen height
        }
*/
screenStruct captureScreenMat(HWND hwnd){
    Mat src;

    // Get handles to a device context (DC)
    HDC hwindowDC = GetDC(hwnd);
    HDC hwindowCompatibleDC = CreateCompatibleDC(hwindowDC);
    SetStretchBltMode(hwindowCompatibleDC, COLORONCOLOR);

    // Define scale, height and width
    int screenx = 0;
    int screeny = 0;
    int width = GetSystemMetrics(SM_CXSCREEN);
    int height = GetSystemMetrics(SM_CYSCREEN);

    // Create mat object
    src.create(height, width, CV_8UC4);

    // Create a bitmap
    HBITMAP hbwindow = CreateCompatibleBitmap(hwindowDC, width, height);
    BITMAPINFOHEADER bi = createBitmapHeader(width, height);

    // Use the previously created device context with the bitmap
    SelectObject(hwindowCompatibleDC, hbwindow);

    // Copy from the window device context to the bitmap device context
    StretchBlt(hwindowCompatibleDC, 0, 0, width, height, hwindowDC, screenx, screeny, width, height, SRCCOPY);
    GetDIBits(hwindowCompatibleDC, hbwindow, 0, height, src.data, (BITMAPINFO*)&bi, DIB_RGB_COLORS);


    // When we copied the screenshot in the "src" variable, free all the bitmap allocated
    DeleteObject(hbwindow);
    DeleteDC(hwindowCompatibleDC);
    ReleaseDC(hwnd, hwindowDC);

    return { src, width, height };
}

/*
    This function create the struct containing the image object and all the necessary infos related to it

    IN: -------------------------------------------------------------
        hwnd: the url to the object

    OUT: ------------------------------------------------------------
        all the infos about the image:{
            the image
            RGB values
            its width
            its height
        }
*/
imageStruct captureImageMat(string imgUrl) {
    // Stock the image in a variable
    Mat image = imread(imgUrl);

    // Get all the datas
    int imgr1 = image.at<Vec3b>(0, 0)[2];
    int imgg1 = image.at<Vec3b>(0, 0)[1];
    int imgb1 = image.at<Vec3b>(0, 0)[0];
    int imgcols = image.cols;
    int imgrows = image.rows;

    return { image, imgr1, imgg1, imgb1, imgcols, imgrows };
}

/*
    This function check for a full match between the target image and the bigger source image (or screen)
    !! The big image (haystack) need to be of screenStruct type (whether it's a screen capture or not)

    IN: -------------------------------------------------------------
        the screen or the main image (the haystack)
        the target image (the needle)
        the x haystack axis
        the y haystack axis

    OUT: ------------------------------------------------------------
        coordinate of the target image (the needle) if found on the haystack {x1, x2, y1, y2, xMiddle, yMiddle}
        representing the full rectangle coordinate: {
            (x1,y1) ---- (x1,y1)
               |            |
               |            |
               |            |
            (x1,y2) -----(x2,y2)
        }

        the point of coordinate (xMiddle, yMiddle) is the center of the image
        (IE: setCursorPos(yMiddle, xMiddle) will set the position of the cursor at the middle of the target image)
*/
coordinate2D checkForCompleteMatch(screenStruct screen, imageStruct img, int haystackI, int haystackJ) {
    // Check every pixel of the target image (the needle) to check if it correspond
    for (int i = 0; i < img.image.rows - 1; i++) {
        for (int j = 0; j < img.image.cols - 1; j++) {
            // Check if both images' pixels correspond to one another
            int rscreen = screen.screenInfo.data[screen.screenInfo.channels() * (screen.screenInfo.cols * (haystackI + i) + (haystackJ + j)) + 2];
            int gscreen = screen.screenInfo.data[screen.screenInfo.channels() * (screen.screenInfo.cols * (haystackI + i) + (haystackJ + j)) + 1];
            int bscreen = screen.screenInfo.data[screen.screenInfo.channels() * (screen.screenInfo.cols * (haystackI + i) + (haystackJ + j)) + 0];
            int rimage = img.image.data[img.image.channels() * (img.image.cols * i + j) + 2];
            int gimage = img.image.data[img.image.channels() * (img.image.cols * i + j) + 1];
            int bimage = img.image.data[img.image.channels() * (img.image.cols * i + j) + 0];

            // If not a matching pixel -> return an "error" coordinate2D
            if (rscreen != rimage || gscreen != gimage || bscreen != bimage) { return {-1, -1, -1, -1}; }
        }
    }
    return { 
        haystackI,
        haystackJ,
        haystackI + img.image.rows,
        haystackJ + img.image.cols,
        (haystackI + haystackI + img.image.rows )/2,
        (haystackJ + haystackJ + img.image.cols)/2
    };
}


list<coordinate2D> findMatchingPixelOnScreen(screenStruct screen, imageStruct img) {
    list <coordinate2D> coordinatesList;
    // Check every pixel to find a potential match
    for (int i = 0; i < screen.screenHeight; i++) {
        for (int j = 0; j < screen.screenWidth; j++) {
            int r = screen.screenInfo.data[screen.screenInfo.channels() * (screen.screenInfo.cols * i + j) + 2];
            int g = screen.screenInfo.data[screen.screenInfo.channels() * (screen.screenInfo.cols * i + j) + 1];
            int b = screen.screenInfo.data[screen.screenInfo.channels() * (screen.screenInfo.cols * i + j) + 0];

            // If detect a matching first pixel (IE: if we see a potential match) -> Check if it match perfectly
            if (r == img.r1 && g == img.g1 && b == img.b1) {
                coordinate2D newCoordinate = checkForCompleteMatch(screen, img, i, j);
                if (newCoordinate.x1 != -1 \
                    && newCoordinate.y1 != -1 \
                    && newCoordinate.x2 != -1 \
                    && newCoordinate.y2 != -1) {
                    coordinatesList.push_back(newCoordinate);
                }
            }
        }
    }
    return coordinatesList;
}

list <coordinate2D> locateOnScreen(string imgUrl) {
    // capture image
    HWND hwnd = GetDesktopWindow();
    screenStruct src = captureScreenMat(hwnd);
    imageStruct img = captureImageMat(imgUrl);

    return findMatchingPixelOnScreen(src, img);
}

int main()
{
    list <coordinate2D> coordinateList = locateOnScreen("C:/Users/tulki/Documents/Projects/Waven/img/Sanstitre.png");

    coordinate2D imageFront = coordinateList.front();

    SetCursorPos(imageFront.yMiddle, imageFront.xMiddle);
    return 0;
}