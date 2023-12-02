#include "CppAutoGUI.h"
#include "Libraries.h"
#include "Scanner.h"
#include "Image.h"
#include "Screenshot.h"
#include "ImageSource.h"


int main()
{
    coordinate2D imageFront1;

    //auto start = chrono::high_resolution_clock::now();
    //auto stop = chrono::high_resolution_clock::now();
    //auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    //cout << duration.count() << "\n";
    Image Haystack = Screenshot();
    Image Needle = ImageSource("C:/Users/Tulkii/Pictures/Screenshots/cap.png");
    Scanner scanner = Scanner();
    std::cout << scanner.locateOnScreen(Haystack, Needle).size() << "\n";
    imageFront1 = scanner.locateOnScreen(Haystack, Needle).front();
    SetCursorPos(imageFront1.yMiddle, imageFront1.xMiddle);

    // 66.000 microsec for png needle file on screen
    return 0;
}
