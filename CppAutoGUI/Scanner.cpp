
#include "Scanner.h"

bool autoGUI::Scanner::isWithinInterval(double value, double target, double interval) {
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
coordinate2D autoGUI::Scanner::checkForCompleteMatch(Image* const Haystack, Image* const Needle, int const haystackI, int const haystackJ) {
    // Check every pixel of the target image (the needle) to check if it correspond
    int truehaystackI = haystackI - Needle->firstI;
    int truehaystackJ = haystackJ - Needle->firstJ;
    auto HaystackData = Haystack->image.data;
    auto NeedleData = Needle->image.data;
    int HaystackChannels = Haystack->image.channels();
    int NeedleChannels = Needle->image.channels();
    int NeedleHeight = Needle->image.rows;
    int NeedleWidth = Needle->image.cols;
    int HaystackWidth = Haystack->image.cols;

    for (int i = 0; i < NeedleHeight - 1; i++) {
        if (haystackI + i >= GetSystemMetrics(SM_CYSCREEN)) { return { -1, -1, -1, -1 }; }
        for (int j = 0; j < NeedleWidth - 1; j++) {
            // Check if both images' pixels correspond to one another
            int HaystackTargetPixel = HaystackChannels * (HaystackWidth * (truehaystackI + i) + (truehaystackJ + j));
            int NeedleTargetPixel = NeedleChannels * (NeedleWidth * i + j);
            if (NeedleData[NeedleTargetPixel + 3] == 0) { continue; }
            int rHaystack = HaystackData[HaystackTargetPixel + 2];
            int gHaystack = HaystackData[HaystackTargetPixel + 1];
            int bHaystack = HaystackData[HaystackTargetPixel + 0];
            int rNeedle = NeedleData[NeedleTargetPixel + 2];
            int gNeedle = NeedleData[NeedleTargetPixel + 1];
            int bNeedle = NeedleData[NeedleTargetPixel + 0];

            // If not a matching pixel -> return an "invalid" coordinate2D
            if (!(isWithinInterval(rHaystack, rNeedle, 1.0) || isWithinInterval(gHaystack, gNeedle, 1.0) || isWithinInterval(bHaystack, bNeedle, 1.0)))
            {
                return { -1, -1, -1, -1 };
            }
        }
    }
    return {
        truehaystackI,
        truehaystackJ,
        truehaystackI + Needle->image.rows,
        truehaystackJ + Needle->image.cols,
        (truehaystackI + truehaystackI + Needle->image.rows) / 2,
        (truehaystackJ + truehaystackJ + Needle->image.cols) / 2
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

std::list<coordinate2D> autoGUI::Scanner::findMatchingPixelOnScreen(Image* const Haystack, Image* const Needle) {
    std::list<coordinate2D> coordinatesList;
    int HaystackHeight = Haystack->imageHeight;
    int HaystackWidth = Haystack->imageWidth;
    int NeedleR1 = Needle->r1;
    int NeedleG1 = Needle->g1;
    int NeedleB1 = Needle->b1;
    int channels = Haystack->image.channels();
    // Check every pixel to find a potential match
    for (int i = 0; i < HaystackHeight; i++) {
        for (int j = 0; j < HaystackWidth; j++) {
            int r = Haystack->image.data[channels * (HaystackWidth * i + j) + 2];
            int g = Haystack->image.data[channels * (HaystackWidth * i + j) + 1];
            int b = Haystack->image.data[channels * (HaystackWidth * i + j) + 0];

            // If detect a matching first pixel (IE: if we see a potential match) -> Check if it match perfectly
            if (isWithinInterval(r, NeedleR1, 1.0) && isWithinInterval(g, NeedleG1, 1.0) && isWithinInterval(b, NeedleB1, 1.0)) {
                // Add a new check for pixel is checked in a new thread if enough threads are available
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
std::list<coordinate2D> autoGUI::Scanner::locateOnScreen(Image* Haystack, Image* Needle) {
    return findMatchingPixelOnScreen(Haystack, Needle);
}