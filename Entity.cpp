#include "Entity.h"
#include "Game.h"

void Entity::WrapCoords()
{
	int sw = Game::SCREEN_WIDTH;
	int sh = Game::SCREEN_HEIGHT;

	if (pos_x < 0.0 - width)
	{
		pos_x += sw + (double)width;
	}
	if (pos_x > sw)
	{
		pos_x -= sw + (double)width;
	}
	if (pos_y < 0.0 - height)
	{
		pos_y += sh + (double)height;
	}
	if (pos_y > sh)
	{
		pos_y -= sh + (double)height;
	}
}

bool Entity::Collision(Entity& other_entity)
{
	//c2 = a2 + b2

	float ent1_cx = pos_x + center.x;
	float ent1_cy = pos_y + center.y;

	float ent2_cx = other_entity.pos_x + other_entity.center.x;
	float ent2_cy = other_entity.pos_y + other_entity.center.y;

	float distance2 = sqrt(fabs(powf(ent2_cx - ent1_cx, 2) + powf(ent2_cy - ent1_cy, 2)));
	float radius2 = radius + other_entity.radius;

	if (radius2 >= distance2)
	{
		return true;
	}

	return false;
}