#pragma once
#include "Entity.h"

class BlackHole : public Entity
{
public:
	float angle_modifier = 0;
	SDL_Texture* img = nullptr;
	bool enlarge, deflate;

	BlackHole(const int screen_width, const int screen_height, const float size_multiplier, const float screen_ratio);

	void Draw() const override;
	void Update(const float dt) override;
	void NewPosition(const int screen_width, const int screen_height);
	void CalculateCentreOffset();
	void InterpCentre(float dt, float total_interp_secs);
	~BlackHole(); // clear rects

private:
	SDL_Texture* rad_img = nullptr;
	SDL_Rect* radius_rect = nullptr;
	float s_w, s_h;
	std::pair <float, float> offset = std::make_pair(0.0f, 0.0f);
	std::pair <float, float> curr_offset = std::make_pair(0.0f, 0.0f);
	std::pair <float, float> orig_size = std::make_pair(0.0f, 0.0f);
	std::pair <float, float> enlarge_offset = std::make_pair(0.0f, 0.0f);
	std::pair <float, float> orig_pos = std::make_pair(0.0f, 0.0f);
};