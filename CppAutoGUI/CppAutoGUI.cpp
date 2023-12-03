#include "CppAutoGUI.h"
#include "Libraries.h"
#include "Scanner.h"
#include "Image.h"
#include "Screenshot.h"
#include "ImageSource.h"


int main()
{
    coordinate2D imageFront1;

    //auto start = std::chrono::high_resolution_clock::now();
    //auto stop = std::chrono::high_resolution_clock::now();
    //auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    //std::cout << duration.count() << "\n";
    Image Haystack = Screenshot();
    Image Needle = ImageSource("C:/Users/Tulkii/Pictures/Screenshots/mr.png");
    Scanner scanner = Scanner();
    imageFront1 = scanner.locateOnScreen(Haystack, Needle).front();
    SetCursorPos(imageFront1.yMiddle, imageFront1.xMiddle);

    // 66.000 microsec for png needle file on screen
    return 0;
}
