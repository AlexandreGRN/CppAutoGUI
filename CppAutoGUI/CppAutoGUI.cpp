#include "CppAutoGUI.h"
#include <chrono>
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
    autoGUI::ImageSource Needle ("C:/Users/Tulkii/Pictures/Screenshots/az.png");
    autoGUI::Screenshot Haystack;
    autoGUI::Scanner scanner;

    imageFront1 = scanner.locateOnScreen(&Haystack, &Needle).front();
    SetCursorPos(imageFront1.yMiddle, imageFront1.xMiddle);
    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
    std::cout << duration.count() << "\n";
    
    return 0;
}
