#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <Windows.h>
#include <string>
#include <typeinfo>
#include <opencv2/highgui.hpp>
#include <filesystem>
#include <thread>
#include <future>
struct coordinate2D {
    int x1;
    int y1;
    int x2;
    int y2;
    int xMiddle;
    int yMiddle;
};