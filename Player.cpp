#include "Player.h"
#include "Game.h"
//#include "TextureManager.h"

Player::Player(double x, double y)
{
	pos_x = x;
	pos_y = y;

	img = Game::game_images[Game::eImages::SHIP];
	rect = Game::GetRect(img, x, y);

	width = rect->w;
	height = rect->h;

	center.x = rect->w * .5;
	center.y = rect->h * .5;
}


void Player::Draw()
{
	SDL_RenderCopyEx(Game::gRenderer, img, NULL, rect, angle, &center, SDL_FLIP_NONE);
}


void Player::Update(double const& dt)
{
	// Cap velocity and scale if limit reached
	double distance = sqrt((pow(vel_x, 2) + pow(vel_y, 2)));
	if (distance > MAX_SPEED)
	{
		vel_x = (vel_x / distance) * MAX_SPEED;
		vel_y = (vel_y / distance) * MAX_SPEED;
	}

	pos_x += vel_x * dt;
	pos_y += vel_y * dt;

	rect->x = pos_x;
	rect->y = pos_y;
}

Player::~Player()
{
	std::cout << "Deleted Player Object" << std::endl;
	delete rect;
	rect = nullptr;
}