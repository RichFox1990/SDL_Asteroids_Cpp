#pragma once
#include "Entity.h"
#include "DelayTimer.h"
#include <utility>


class PowerUp : public Entity
{
public:

	PowerUp(const Entity* const ast, const float screen_ratio);
	void HandleDamage();
	void Draw() const override;
	void Update(const float dt) override;
	
	~PowerUp(); // clear rects

private:
	SDL_Texture* outer_img = nullptr;
	SDL_Rect* outer_rect = nullptr;
	SDL_Texture* img = nullptr;
	DelayTimer life_time{ 25000.0f, false };
	DelayTimer flash{ 150.0f, true };
	DelayTimer pulse{ 1000.0f, false };
	int MAX_SPEED;
	bool draw = true;
	float progress_offset = 0.0f;
	std::pair <float, float> orig_size;
	std::pair <float, float> pulse_offset = std::make_pair(0.0f, 0.0f);
};

