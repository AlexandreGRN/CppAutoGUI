
#include "Scanner.h"
#include "Libraries.h"

Scanner::Scanner() {
    
}

bool Scanner::isWithinInterval(double value, double target, double interval) {
    return (std::fabs(value - target) <= interval);
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
            (x1,y2) ---- (x2,y2)
        }

        the point of coordinate (xMiddle, yMiddle) is the center of the image
        (IE: setCursorPos(yMiddle, xMiddle) will set the position of the cursor at the middle of the target image)
*/
coordinate2D Scanner::checkForCompleteMatch(Image Haystack, Image Needle, int haystackI, int haystackJ) {
    // Check every pixel of the target image (the needle) to check if it correspond
    int truehaystackI = haystackI - Needle.firstI;
    int truehaystackJ = haystackJ - Needle.firstJ;
    for (int i = 0; i < Needle.image.rows - 1; i++) {
        if (haystackI + i >= GetSystemMetrics(SM_CYSCREEN)) { return { -1, -1, -1, -1 }; }
        for (int j = 0; j < Needle.image.cols - 1; j++) {
            if (Needle.image.data[Needle.image.channels() * (Needle.image.cols * i + j) + 3] == 0) { continue; }
            // Check if both images' pixels correspond to one another
            int rHaystack = Haystack.image.data[Haystack.image.channels() * (Haystack.image.cols * (truehaystackI + i) + (truehaystackJ + j)) + 2];
            int gHaystack = Haystack.image.data[Haystack.image.channels() * (Haystack.image.cols * (truehaystackI + i) + (truehaystackJ + j)) + 1];
            int bHaystack = Haystack.image.data[Haystack.image.channels() * (Haystack.image.cols * (truehaystackI + i) + (truehaystackJ + j)) + 0];
            int rNeedle = Needle.image.data[Needle.image.channels() * (Needle.image.cols * i + j) + 2];
            int gNeedle = Needle.image.data[Needle.image.channels() * (Needle.image.cols * i + j) + 1];
            int bNeedle = Needle.image.data[Needle.image.channels() * (Needle.image.cols * i + j) + 0];

            // If not a matching pixel -> return an "error" coordinate2D
            if (!(isWithinInterval(rHaystack, rNeedle, 10.0)  || isWithinInterval(gHaystack, gNeedle, 10.0) || isWithinInterval(bHaystack, bNeedle, 10.0)))
            {
                return { -1, -1, -1, -1 };
            }
        }
    }
    return {
        truehaystackI,
        truehaystackJ,
        truehaystackI + Needle.image.rows,
        truehaystackJ + Needle.image.cols,
        (truehaystackI + truehaystackI + Needle.image.rows) / 2,
        (truehaystackJ + truehaystackJ + Needle.image.cols) / 2
    };
}

/*
    This function find all the potential matches, it check every pixel of the screen/big image (the haystack)
    if a certain pixel of the screen/big image (the haystack) completly correspond to the first pixel of the target image (the needle), call the function checkForCompleteMatch
    If the match is complete: Stock the coordinate in a list, if not, skip to the next pixel
    !! The big image (haystack) need to be of screenStruct type (whether it's a screen capture or not)

    IN: -------------------------------------------------------------
       the screen or the main image (the haystack)
       the target image (the needle)

    OUT: ------------------------------------------------------------
        The list of coordinates of all the occurence:
        List of type <coordinate2D>

*/
std::list<coordinate2D> Scanner::findMatchingPixelOnScreen(Image Haystack, Image Needle) {
    std::list<coordinate2D> coordinatesList;
    // Check every pixel to find a potential match
    for (int i = 0; i < Haystack.imageHeight; i++) {
        for (int j = 0; j < Haystack.imageWidth; j++) {
            int r = Haystack.image.data[Haystack.image.channels() * (Haystack.image.cols * i + j) + 2];
            int g = Haystack.image.data[Haystack.image.channels() * (Haystack.image.cols * i + j) + 1];
            int b = Haystack.image.data[Haystack.image.channels() * (Haystack.image.cols * i + j) + 0];

            // If detect a matching first pixel (IE: if we see a potential match) -> Check if it match perfectly
            if (isWithinInterval(r, Needle.r1, 1.0) && isWithinInterval(g, Needle.g1, 1.0) && isWithinInterval(b, Needle.b1, 1.0)) {
                coordinate2D newCoordinate = checkForCompleteMatch(Haystack, Needle, i, j);
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

/*
    This function locate an image (the needle) in the screen (the haystack)

    IN: -------------------------------------------------------------
        The target image (the needle) path (exemple: C:/Users/<user>/Documents/image.png)

    OUT: ------------------------------------------------------------
        The list of coordinates of all the occurence:
            List of type <coordinate2D>

            coordinate2D: {
            (x1,y1) ---- (x1,y1)
               |            |
               |            |
               |            |
            (x1,y2) ---- (x2,y2)
        }

        the point of coordinate (xMiddle, yMiddle) is the center of the image
        (IE: setCursorPos(yMiddle, xMiddle) will set the position of the cursor at the middle of the target image)

        -------------------------------------------------------------
        In order to set the position of your cursor onto the middle of an image:
        Use:
            SetCursorPos(List<coordinate2D>.front().yMiddle, List<coordinate2D>.front().xMiddle)
*/
std::list<coordinate2D> Scanner::locateOnScreen(Image Haystack, Image Needle) {
    return findMatchingPixelOnScreen(Haystack, Needle);
}
