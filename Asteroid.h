#pragma once
#include "Entity.h"
class Asteroid : public Entity
{
public:
	double angle_modifier = 0;
	SDL_Texture* img = nullptr;
	Asteroid(double x, double y, double vx, double vy, int angle, float asteriod_size, double rand_img, bool isCollidable, float screen_ratio);

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
	SDL_Texture* rad_img = nullptr;
	SDL_Rect* radius_rect = nullptr;
	int MAX_SPEED;
};

