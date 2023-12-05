#pragma once
#include "Libraries.h"
#include "image.h"

// ImageSource not supposed to change throughout the program -> constexpr class | Not the case for Screenshot
constexpr class ImageSource : public Image
{
	// Methods
public:
	ImageSource(const std::string imgPath);
private:
	void captureImageMat(std::string imgPath);

};