#pragma once
#include "Entity.h"
class Asteroid : public Entity
{
public:
	double angle_modifier = 0;
	Asteroid(double x, double y, double vx, double vy, int angle, int asteriod_scale, double rand_img, bool isCollidable);

	void Draw() override;
	void Update(double const& dt) override;
	~Asteroid(); // clear rects

	enum SIZE
	{
		SMALL = 2,
		MEDIUM = 4,
		LARGE = 6
	};

private:
	//SDL_Texture* rad_img = nullptr;
	//SDL_Rect* radius_rect = nullptr;
	SDL_Texture* img = nullptr;
	const double MAX_SPEED = 350.0;
};

