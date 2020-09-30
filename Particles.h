#pragma once
#include <iostream>
#include <vector>
#include "SDL.h"
#include "DelayTimer.h"

class Player;

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
	Particles(const int x, const int y, double radians, const double speed, const double offset, const int amount, const float s_r);
	Particles(const int x, const int y, const float amount, const float s_r);
	Particles(Player* player, const int amount, const float s_r);
	void Update(double const dt);
	void Draw() const;

	int amount = 0;
	double delta_time = 0;
	mutable int opacity = 255;
	mutable double time_elapsed = 1.0;
	//int radius = 0;
	float SPEED = 40.0f;
	float angle_modifier = 2.0f * 3.14159f;
	
	DelayTimer life_cycle = { 1250, false };
	bool is_complete = false;

	std::vector<std::unique_ptr<Pixel>> pixels;
	//std::vector<Pixel*> pixels;
	
};

