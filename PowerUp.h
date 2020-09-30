#pragma once
#include "Entity.h"

class PowerUp : public Entity
{
public:

	PowerUp(const double x, const double y, const float screen_ratio);
	void Draw() const override;
	void Update(const double dt) override;
	~PowerUp(); // clear rects

private:
	SDL_Texture* outer_img = nullptr;
	SDL_Rect* outer_rect = nullptr;
	SDL_Texture* img = nullptr;

};

