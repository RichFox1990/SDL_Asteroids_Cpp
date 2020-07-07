#pragma once
#include "Point.h"
#include <iostream>
#include "SDL.h"

class Entity
{
public:
	int angle = 0;
	Point pos{ 0, 0 };
	SDL_Point center;
	Point vel{ 0, 0 };
	bool is_dead = false;

	Entity(double x = 0, double y = 0)
	{
		pos.x = x; pos.y = y;
		center.x = 0;
		center.y = 0;
	}

	virtual ~Entity() { std::cout << "deleted Entity - OVERRIDE THIS" << std::endl; };

	virtual void Update(double const& dt) = 0;
	virtual void Draw() = 0;

};

