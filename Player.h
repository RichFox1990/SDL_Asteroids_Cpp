#pragma once
#include <iostream>
#include "Entity.h"
#include "SDL.h"
#include "DelayTimer.h"

class Player : public Entity
{
public:
	bool debug = false;
	bool damaged = false;
	int to_rotate = 0;
	float current_sheild;
	SDL_Texture* img = nullptr;
	std::unique_ptr<Entity>* asteroid_death = nullptr;

	Player(const double x, const double y, const float screen_ratio);

	void Draw() const override;
	void Update(const double dt) override;
	SDL_Texture* getImage() const;
	void setImage(SDL_Texture* const new_img);
	void Damage(const float amount);

	~Player();

private:
	SDL_Texture* rad_img = nullptr;
	SDL_Rect* radius_rect;

	DelayTimer flash{ 125.0f, true };
	bool draw = true;

	int MAX_SPEED;
	const int MAX_ANGLE_MODIFIER = 350;
};

