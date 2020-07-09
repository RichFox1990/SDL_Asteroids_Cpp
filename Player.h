#pragma once
#include "Entity.h"
#include "SDL.h"

class Player : public Entity
{
public:
	Player(double x = 0.0, double y = 0.0);

	void WrapCoords();
	void Draw() override;
	void Update(double const& dt) override;
	~Player(); // clear rects

private:
	SDL_Texture* img = nullptr;
	const double MAX_SPEED = 350.0;
};

