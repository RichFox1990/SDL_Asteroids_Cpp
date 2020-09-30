#include "Bullet.h"
#include "Game.h"
#include "Asteroid.h"


Bullet::Bullet(const Entity* player, const float s_r)
{

	MAX_SPEED = 1000 * s_r;
	angle = player->angle;
	pos_x = sin((double)angle * PI / 180.0) * player->height / 4 + player->rect->x + player->width / 2;
	pos_y = -cos((double)angle * PI / 180.0) * player->height / 4 + player->rect->y + player->height / 2;

	// bullet rect (5x5)
	rect = new SDL_Rect{ (int)pos_x, (int)pos_y, (int)((Asteroid::SMALL * 3.0f) * s_r), (int)((Asteroid::SMALL * 3.0f) * s_r) };
	width = rect->w;
	height = rect->h;
	// set bullet velocity
	vel_x = sin((double)angle * PI / 180.0) * MAX_SPEED;
	vel_y = -cos((double)angle * PI / 180.0) * MAX_SPEED;

	radius = rect->h * .5;
	center.x = rect->w * .5;
	center.y = rect->h * .5;
}

void Bullet::WrapCoords(const int sw, const int sh)
{
	if (pos_x < 0.0 - width)
	{
		is_dead = true;
	}
	if (pos_x > sw)
	{
		is_dead = true;
	}
	if (pos_y < 0.0 - height)
	{
		is_dead = true;
	}
	if (pos_y > sh)
	{
		is_dead = true;
	}
}

void Bullet::Update(const double dt)
{
	pos_x += vel_x * dt;
	pos_y += vel_y * dt;

	rect->x = pos_x;
	rect->y = pos_y;
}

void Bullet::Draw() const
{
	SDL_SetRenderDrawColor(Game::gRenderer, 0, 255, 0, 255);
	SDL_RenderFillRect(Game::gRenderer, rect);
}

Bullet::~Bullet()
{
	//std::cout << "deleted Bullet" << std::endl;
	delete rect;
	rect = nullptr;
}