#pragma once
#include "SDL.h"

class Entity
{
public:
	Entity() {}
	virtual ~Entity() {};
	// Methods
	virtual bool Collision(Entity& other_entity);
	virtual void WrapCoords(int screen_width, int screen_height);
	virtual void Update(double const& dt) = 0;
	virtual void Draw() = 0;

	//Variables
	int angle = 0;
	int width = 0;
	int height = 0;
	float size = 0.0f; // used for asteroids

	bool is_dead = false;
	bool isCollidable = true;

	double pos_x = 0;
	double pos_y = 0;
	double vel_x = 0;
	double vel_y = 0;

	double radius = 0;

	SDL_Point center{ 0, 0 };
	SDL_Rect* rect = nullptr;

};

