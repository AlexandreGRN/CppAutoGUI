#pragma once
#include "image.h"

namespace autoGUI
{
	class Screenshot : public Image
	{
	public:
		Screenshot();
		~Screenshot();
		Screenshot(const Screenshot&) = delete;
		Screenshot(Screenshot&&) = delete;

	private:

		// Create the Bitmap Header that contains all the infos about the screenshot
		BITMAPINFOHEADER createBitmapHeader(int width, int height);

		// Create the screenshot object of type cv::Mat
		void captureScreenMat(HWND hwnd);
	};
}