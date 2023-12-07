#pragma once
#include "image.h"

class Screenshot : public Image
{
public:
	Screenshot();
private:
	BITMAPINFOHEADER createBitmapHeader(int width, int height);
	void captureScreenMat(HWND hwnd);
};