#pragma once
#include <iostream>
#include <vector>
#include "SDL.h"
#include "DelayTimer.h"

class Entity;

struct Pixel
{
	SDL_Rect rect;
	float pos_x;
	float pos_y;
	float vel_x;
	float vel_y;
};

class Particles
{
public:
	Particles(const int x, const int y, float radians, const float speed, const float offset, const int amount, const float s_r); // larger ast explosion (directional)
	Particles(const int x, const int y, const float amount, const float s_r); // small ast explosion
	Particles(Entity* player, const int amount, const float s_r); // player thrust
	Particles(Entity* player, const float s_r); // player hit
	void Update(float const dt);
	void Draw() const;

	int amount = 0;
	float delta_time = 0;
	mutable int opacity = 255;
	mutable float time_elapsed = 1.0;
	//int radius = 0;
	float SPEED = 40.0f;
	float angle_modifier = 2.0f * 3.14159f;
	
	DelayTimer life_cycle = { 1250, false };
	bool is_complete = false;
	char ent_type = NULL;

	std::vector<std::unique_ptr<Pixel>> pixels;
	//std::vector<Pixel*> pixels;
	
};

