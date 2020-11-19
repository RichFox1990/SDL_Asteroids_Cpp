#pragma once
#include "SDL.h"

class Entity
{
public:
	Entity() {}
	// Methods
	virtual bool Collision(Entity& other_entity) const;
	virtual void WrapCoords(int screen_width, int screen_height);
	virtual void Update(float const dt) = 0;
	virtual void Draw() const = 0;

	//Variables
	int angle = 0;
	int width = 0;
	int height = 0;
	float size = 0.0f; // used for asteroids

	bool to_remove = false;
	bool draw = true;
	bool isCollidable = true;
	bool debug = false;

	float pos_x = 0;
	float pos_y = 0;
	float vel_x = 0;
	float vel_y = 0;

	float radius = 0;

	SDL_Point center{ 0, 0 };
	SDL_Rect* rect = nullptr;

};

