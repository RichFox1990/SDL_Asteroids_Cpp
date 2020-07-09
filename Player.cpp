#include "Player.h"
#include "Game.h"

Player::Player(double x, double y)
{
	pos.x = x;
	pos.y = y;
	is_dead = false;
	img = Game::game_images[Game::eImages::SHIP]->texture;
	rect = Game::game_images[Game::eImages::SHIP]->GetRect(x, y);

	width = rect->w;
	height = rect->h;

	center.x = rect->w * .5;
	center.y = rect->h * .5;

	wrap_coords = true;
}

void Player::WrapCoords()
{
	if (pos.x < 0.0 - width)
	{
		pos.x = Game::SCREEN_WIDTH;
	}
	if (pos.x > Game::SCREEN_WIDTH)
	{
		pos.x = 0.0 - width;
	}
	if (pos.y < 0.0 - height)
	{
		pos.y = Game::SCREEN_HEIGHT;
	}
	if (pos.y > Game::SCREEN_HEIGHT)
	{
		pos.y = 0.0 - height;
	}
}


void Player::Draw()
{
	SDL_RenderCopyEx(Game::gRenderer, img, NULL, rect, angle, &center, SDL_FLIP_NONE);
}


void Player::Update(double const& dt)
{
	// Cap velocity and scale if limit reached
	double distance = sqrt((pow(vel.x, 2) + pow(vel.y, 2)));
	if (distance > MAX_SPEED)
	{
		vel.x = (vel.x / distance) * MAX_SPEED;
		vel.y = (vel.y / distance) * MAX_SPEED;
	}

	pos.x += vel.x * dt;
	pos.y += vel.y * dt;

	rect->x = pos.x;
	rect->y = pos.y;
}

Player::~Player()
{
	std::cout << "Deleted Player Object" << std::endl;
	delete rect;
	rect = nullptr;
}