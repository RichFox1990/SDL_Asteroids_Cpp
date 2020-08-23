#pragma once
#include "Entity.h"
class Asteroid : public Entity
{
public:
	double angle_modifier = 0;
	SDL_Texture* img = nullptr;

	Asteroid(const double x, const double y, const double vx, const double vy, const int angle, float asteriod_size, const bool isCollidable, const float screen_ratio);

	void Draw() const override;
	void Update(const double dt) override;
	~Asteroid(); // clear rects

	enum SIZE
	{
		SMALL = 1,
		MEDIUM = 2,
		LARGE = 3
	};

private:
	SDL_Texture* rad_img = nullptr;
	SDL_Rect* radius_rect = nullptr;
	int MAX_SPEED;
};

