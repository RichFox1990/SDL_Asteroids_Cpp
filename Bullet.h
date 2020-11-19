#pragma once
#include "Entity.h"

class Bullet : public Entity
{
public:

	Bullet(const Entity* player, const float s_r);
	void WrapCoords(const int screen_width, const int screen_height);
	void Draw() const override;
	void Update(const float dt) override;
	~Bullet();

private:
	int MAX_SPEED;
};

