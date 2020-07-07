#pragma once
#include <iostream>
#include <string>
#include "ImageData.h"

class TextureManager
{
public:
	static ImageData* load_image_data(std::string path, bool& allMediaLoaded);
};

