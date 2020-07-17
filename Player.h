#pragma once
#include <iostream>
#include "Entity.h"
#include "SDL.h"
#include "DelayTimer.h"

class Player : public Entity
{
public:
	bool debug = false;
	bool damaged = false;
	int to_rotate = 0;

	Player(double x, double y, float screen_ratio);

	void Draw() override;
	void Update(double const& dt) override;
	SDL_Texture* img = nullptr;
	std::unique_ptr<Entity>* asteroid_death = nullptr;
	~Player(); // clear rects

private:
	SDL_Texture* rad_img = nullptr;
	SDL_Rect* radius_rect;

	DelayTimer flash{ 125.0f, true };
	bool draw = true;

	int MAX_SPEED;
	const int MAX_ANGLE_MODIFIER = 350;
};

