#include "Screenshot.h"

autoGUI::Screenshot::Screenshot() : Image()
{
    captureScreenMat(GetDesktopWindow());
}

autoGUI::Screenshot::~Screenshot()
{
    this->image.release();
}

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
BITMAPINFOHEADER autoGUI::Screenshot::createBitmapHeader(int const width, int const height) {
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
void autoGUI::Screenshot::captureScreenMat(HWND hwnd) {
    cv::Mat src;

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

    this->image = src;
    this->imageWidth = width;
    this->imageHeight = height;
    this->bIsScreen = true;
}
