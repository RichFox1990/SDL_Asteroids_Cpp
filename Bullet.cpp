#include "Bullet.h"
#include "Game.h"


Bullet::Bullet(Entity* player)
{
	pos.x = sin((double)angle * PI / 180.0) + player->rect->x + player->width / 2;
	pos.y = -cos((double)angle * PI / 180.0) + player->rect->y + player->height / 2;
	angle = player->angle;

	rect = new SDL_Rect{ (int)pos.x, (int)pos.y, 4, 4 };

	vel.x = sin((double)angle * PI / 180.0) * MAX_SPEED*2;
	vel.y = -cos((double)angle * PI / 180.0) * MAX_SPEED*2;

	wrap_coords = false;
}

void Bullet::WrapCoords()
{
	if (pos.x < 0.0 - width)
	{
		is_dead = true;
	}
	if (pos.x > Game::SCREEN_WIDTH)
	{
		is_dead = true;
	}
	if (pos.y < 0.0 - height)
	{
		is_dead = true;
	}
	if (pos.y > Game::SCREEN_HEIGHT)
	{
		is_dead = true;
	}
}

void Bullet::Update(double const& dt)
{
	pos.x += vel.x * dt;
	pos.y += vel.y * dt;

	rect->x = pos.x;
	rect->y = pos.y;
}

void Bullet::Draw()
{
	SDL_SetRenderDrawColor(Game::gRenderer, 0, 255, 0, 255);
	//SDL_RenderDrawPoint(Game::gRenderer, pos.x, pos.y);
	SDL_RenderFillRect(Game::gRenderer, rect);
}

Bullet::~Bullet()
{
	std::cout << "deleted Bullet" << std::endl;
	delete rect;
	rect = nullptr;
}