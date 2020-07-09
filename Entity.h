#pragma once
#include "Point.h"
#include <iostream>
#include "SDL.h"

class Entity
{
public:
	int angle = 0;
	int width;
	int height;

	bool is_dead = false;
	bool wrap_coords = false;

	Point pos{ 0, 0 };
	Point vel{ 0, 0 };
	SDL_Point center;
	SDL_Rect* rect = nullptr;

	Entity(double x = 0, double y = 0)
	{
		pos.x = x; pos.y = y;
		center.x = 0;
		center.y = 0;
		width = 0;
		height = 0;
	}

	virtual void WrapCoords() = 0;

	virtual ~Entity() { /*std::cout << "deleted Entity - OVERRIDE THIS" << std::endl;*/ };

	virtual void Update(double const& dt) = 0;
	virtual void Draw() = 0;

};

