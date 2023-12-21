#pragma once
#include "Libraries.h"
#include "Image.h"


namespace autoGUI
{
	class Scanner
	{
	public:
		std::list<coordinate2D> locateOnScreen(Image* Haystack, Image* Needle);

	private:
		std::list<coordinate2D> findMatchingPixelOnScreen(Image* Haystack, Image* Needle);
		coordinate2D checkForCompleteMatch(Image* Haystack, Image* Needle, int haystackI, int haystackJ);
		bool isWithinInterval(double value, double target, double interval);
	};
}