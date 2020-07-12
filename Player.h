#pragma once
#include "Entity.h"
#include "SDL.h"

class Player : public Entity
{
public:
	Player(double x, double y);

	void Draw() override;
	void Update(double const& dt) override;
	~Player(); // clear rects

private:
	//SDL_Texture* rad_img = nullptr;
	//SDL_Rect* radius_rect;
	SDL_Texture* img = nullptr;
	const double MAX_SPEED = 350.0;
};

