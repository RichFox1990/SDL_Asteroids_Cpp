#pragma once
#include "Entity.h"
class Bullet : public Entity
{
public:

	Bullet(Entity* player, float s_r);
	void WrapCoords(int screen_width, int screen_height);
	void Draw() override;
	void Update(double const& dt) override;
	~Bullet();

private:
	int MAX_SPEED;
};

