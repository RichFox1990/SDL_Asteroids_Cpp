#pragma once
#include "SDL.h"
#include "SDL_image.h"
#include "Point.h"

struct ImageData
{
	SDL_Texture* texture = nullptr;

	//ImageData();
	SDL_Rect* GetRect(int x, int y);
};
