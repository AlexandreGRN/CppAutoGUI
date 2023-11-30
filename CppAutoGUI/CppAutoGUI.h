#pragma once

#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>
#include <string>
#include <typeinfo>
#include <opencv2/highgui.hpp>
#include <chrono>
#include <thread>
#include <filesystem>
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
    int firstI;
    int firstJ;
} IMG;

struct coordinate2D {
    int x1;
    int y1;
    int x2;
    int y2;
    int xMiddle;
    int yMiddle;
};

bool isWithinInterval(double value, double target, double interval);
BITMAPINFOHEADER createBitmapHeader(int width, int height);
screenStruct captureScreenMat(HWND hwnd);
imageStruct captureImageMat(string imgPath);
coordinate2D checkForCompleteMatch(screenStruct screen, imageStruct img, int haystackI, int haystackJ);
list<coordinate2D> findMatchingPixelOnScreen(screenStruct screen, imageStruct img);
list <coordinate2D> locateOnScreen(string imgPath);