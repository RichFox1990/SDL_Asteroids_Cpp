#pragma once
#include "Entity.h"
class Bullet : public Entity
{
public:

	Bullet(Entity* player);
	void WrapCoords();
	void Draw() override;
	void Update(double const& dt) override;
	~Bullet();

private:
	const int MAX_SPEED = 1200;
};

