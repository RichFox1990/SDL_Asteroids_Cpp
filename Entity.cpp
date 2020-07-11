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