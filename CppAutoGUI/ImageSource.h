#pragma once
#include "Libraries.h"
#include "image.h"

class ImageSource : public Image
{
	// Methods
public:
	ImageSource(std::string imgPath);
private:
	void captureImageMat(std::string imgPath);

};