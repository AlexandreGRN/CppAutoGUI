
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
    int truehaystackI = haystackI - Needle.getFirstICoordinate();
    int truehaystackJ = haystackJ - Needle.getFirstJCoordinate();
    auto HaystackData = Haystack.getImage().data;
    auto NeedleData = Needle.getImage().data;
    int HaystackChannels = Haystack.getImage().channels();
    int NeedleChannels = Needle.getImage().channels();
    int NeedleHeight = Needle.getImage().rows;
    int NeedleWidth = Needle.getImage().cols;
    int HaystackWidth = Haystack.getImage().cols;

    for (int i = 0; i < NeedleHeight - 1; i++) {
        if (haystackI + i >= GetSystemMetrics(SM_CYSCREEN)) { return { -1, -1, -1, -1 }; }
        for (int j = 0; j < NeedleWidth - 1; j++) {
            if (Needle.getImage().data[Needle.getImage().channels() * (NeedleWidth * i + j) + 3] == 0) { continue; }
            // Check if both images' pixels correspond to one another
            int HaystackTargetPixel = HaystackChannels * (HaystackWidth * (truehaystackI + i) + (truehaystackJ + j));
            int NeedleTargetPixel = NeedleChannels * (NeedleWidth * i + j);
            int rHaystack = HaystackData[HaystackTargetPixel + 2];
            int gHaystack = HaystackData[HaystackTargetPixel + 1];
            int bHaystack = HaystackData[HaystackTargetPixel + 0];
            int rNeedle = NeedleData[NeedleTargetPixel + 2];
            int gNeedle = NeedleData[NeedleTargetPixel + 1];
            int bNeedle = NeedleData[NeedleTargetPixel + 0];

            // If not a matching pixel -> return an "error" coordinate2D
            if (!(isWithinInterval(rHaystack, rNeedle, 1.0)  || isWithinInterval(gHaystack, gNeedle, 1.0) || isWithinInterval(bHaystack, bNeedle, 1.0)))
            {
                return { -1, -1, -1, -1 };
            }
        }
    }
    return {
        truehaystackI,
        truehaystackJ,
        truehaystackI + Needle.getImage().rows,
        truehaystackJ + Needle.getImage().cols,
        (truehaystackI + truehaystackI + Needle.getImage().rows) / 2,
        (truehaystackJ + truehaystackJ + Needle.getImage().cols) / 2
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
    int HaystackHeight = Haystack.getImageHeight();
    int HaystackWidth = Haystack.getImageWidth();
    int NeedleR1 = Needle.getR1();
    int NeedleG1 = Needle.getG1();
    int NeedleB1 = Needle.getB1();
    int channels = Haystack.getImage().channels();
    auto data = Haystack.getImage().data;
    // Check every pixel to find a potential match
    for (int i = 0; i < HaystackHeight; i++) {
        for (int j = 0; j < HaystackWidth; j++) {
            int r = data[channels * (HaystackWidth * i + j) + 2];
            int g = data[channels * (HaystackWidth * i + j) + 1];
            int b = data[channels * (HaystackWidth * i + j) + 0];

            // If detect a matching first pixel (IE: if we see a potential match) -> Check if it match perfectly
            if (isWithinInterval(r, NeedleR1, 1.0) && isWithinInterval(g, NeedleG1, 1.0) && isWithinInterval(b, NeedleB1, 1.0)) {
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
