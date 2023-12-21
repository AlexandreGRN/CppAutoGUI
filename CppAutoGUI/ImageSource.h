#pragma once
#include "Libraries.h"
#include "image.h"

// ImageSource not supposed to change throughout the program -> constexpr class | Not the case for Screenshot
namespace autoGUI
{
	class ImageSource : public Image
	{
		// Methods
	public:
		ImageSource(std::string imgPath);
		~ImageSource();
		ImageSource(ImageSource&& Copy) noexcept;
		ImageSource(ImageSource& Copy) = default;
	private:
		void captureImageMat(std::string const imgPath);
	};
}
