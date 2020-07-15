#pragma once
#include "Entity.h"
#include "SDL.h"

class Player : public Entity
{
public:
	bool debug = false;
	int to_rotate = 0;

	Player(double x, double y, float screen_ratio);

	void Draw() override;
	void Update(double const& dt) override;
	SDL_Texture* img = nullptr;
	~Player(); // clear rects

private:
	SDL_Texture* rad_img = nullptr;
	SDL_Rect* radius_rect;

	int MAX_SPEED;
	const int MAX_ANGLE_MODIFIER = 350;
};

