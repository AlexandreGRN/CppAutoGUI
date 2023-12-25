#include "CppAutoGUI.h"
#include <chrono>
/*
Speed test using:
auto start = std::chrono::high_resolution_clock::now();
auto stop = std::chrono::high_resolution_clock::now();
auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);
std::cout << duration.count() << "\n";
------------------------------------------------------------
test:
 Single search only locate on screen
fastest: 1.0 microseconds      | Find a small image in the top left corner of the screen
slowest: 180.0 microseconds    | Find a big image in the bottom right corner of the screen with a lot of close match all over the screen

- Extreme values
- Average values out of 1000 concurrent tests on medium-end computer
*/


int main()
{
    // Initialize objects
    autoGUI::ImageSource Needle ("C:/Users/Tulkii/Pictures/Screenshots/az.png");
    autoGUI::Screenshot Haystack;
    autoGUI::Scanner scanner;

    coordinate2D imageFront1 (scanner.locateOnScreen(&Haystack, &Needle).front());
    SetCursorPos(imageFront1.yMiddle, imageFront1.xMiddle);
    
    return 0;
}
