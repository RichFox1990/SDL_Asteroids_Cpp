#pragma once
#include "Entity.h"

class BlackHole : public Entity
{
public:
	double angle_modifier = 0;
	SDL_Texture* img = nullptr;

	BlackHole(const int screen_width, const int screen_height, const float size_multiplier, const float screen_ratio);

	void Draw() const override;
	void Update(const double dt) override;
	void NewPosition(const int screen_width, const int screen_height);
	~BlackHole(); // clear rects

private:
	SDL_Texture* rad_img = nullptr;
	SDL_Rect* radius_rect = nullptr;
	int MAX_SPEED;
};