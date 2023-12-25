#pragma once
#include "Libraries.h"
#include "Image.h"


namespace autoGUI
{
	class Scanner
	{
	public:
		std::vector<coordinate2D> locateOnScreen(Image* Haystack, Image* Needle);

	private:
		void findMatchingPixelOnScreen(Image* Haystack, Image* Needle, int const HeightCheckStart, int const HeightCheckEnd, std::vector<coordinate2D>* coordinateList, std::mutex* mtx);
		coordinate2D checkForCompleteMatch(Image* Haystack, Image* Needle, int haystackI, int haystackJ);
		bool isWithinInterval(double value, double target, double interval);
	};
}