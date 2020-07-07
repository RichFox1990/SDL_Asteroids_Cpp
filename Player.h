#pragma once
#include "Entity.h"
#include "SDL.h"

class Player : public Entity
{
public:
	Player(double x = 0.0, double y = 0.0);
	
	void Draw() override;
	void Update(double const& dt) override;
	~Player(); // clear rects

private:
	SDL_Texture* img = nullptr;
	SDL_Rect* rect = nullptr;
	const int MAX_SPEED = 60;
};

