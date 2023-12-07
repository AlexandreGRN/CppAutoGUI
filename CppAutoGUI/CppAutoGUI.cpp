#include "CppAutoGUI.h"
#include "Libraries.h"
#include "Scanner.h"
#include "Image.h"
#include "Screenshot.h"
#include "ImageSource.h"
#include "ScannerThreadStackHandler.h"
/*
Speed test using:
auto start = std::chrono::high_resolution_clock::now();
auto stop = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
std::cout << duration.count() << "\n";
*/


int main()
{
    coordinate2D imageFront1;

    auto start = std::chrono::high_resolution_clock::now();

    // Initialize objects
    Screenshot Haystack = {};
    ImageSource Needle = { "C:/Users/Tulkii/Pictures/Screenshots/az.png" };
    Scanner scanner = {};
    ScannerThreadStackHandler threadStack = {};

    std::vector<std::thread> threads;

    for (int i = 0; i < 10; i++)
    {
        threadStack.AddNewThreadIfPossible(scanner, Haystack, Needle);
    }

    threadStack.WaitForAllThread();
    imageFront1 = threadStack.GetCoordinates().front();
    unsigned int numThreads = std::thread::hardware_concurrency();
    std::cout << "Number of threads: " << numThreads << std::endl;


    SetCursorPos(imageFront1.yMiddle, imageFront1.xMiddle);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << "\n";
    return 0;
}
