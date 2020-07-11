#pragma once
#include "SDL.h"

class Entity
{
public:
	int angle = 0;
	int width;
	int height;

	bool is_dead = false;
	bool isCollidable = true;

	double pos_x = 0;
	double pos_y = 0;
	double vel_x = 0;
	double vel_y = 0;

	SDL_Point center;
	SDL_Rect* rect = nullptr;

	Entity(double x = 0, double y = 0)
	{
		pos_x = x; pos_y = y;
		center.x = 0;
		center.y = 0;
		width = 0;
		height = 0;
	}

	virtual void WrapCoords();

	virtual ~Entity() { /*std::cout << "deleted Entity - OVERRIDE THIS" << std::endl;*/ };

	virtual void Update(double const& dt) = 0;
	virtual void Draw() = 0;

};

