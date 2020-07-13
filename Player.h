#pragma once
#include "Entity.h"
#include "SDL.h"

class Player : public Entity
{
public:
	bool debug = false;

	Player(double x, double y, float screen_ratio);

	void Draw() override;
	void Update(double const& dt) override;
	SDL_Texture* img = nullptr;
	~Player(); // clear rects

private:
	SDL_Texture* rad_img = nullptr;
	SDL_Rect* radius_rect;

	int MAX_SPEED;
};

