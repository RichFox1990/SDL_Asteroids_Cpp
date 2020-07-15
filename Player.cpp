#include "Player.h"
#include "Game.h"

Player::Player(double x, double y, float s_r)
{
	MAX_SPEED = 350.0 * s_r;
	pos_x = x;
	pos_y = y;

	img = Game::game_images[Game::eImages::SHIP];
	rect = Game::GetRect(img, x, y);
	rect->w *= s_r;
	rect->h *= s_r;

	radius = (rect->h * .5) *.9;
	width = rect->w;
	height = rect->h;

	center.x = rect->w * .5;
	center.y = rect->h * .5;

	rad_img = Game::game_images[Game::eImages::CIRCLE];
	radius_rect = Game::GetRect(rad_img, pos_x + center.x, pos_y + center.y);
	radius_rect->w = radius;
	radius_rect->h = radius;
	radius_rect->x -= radius / 2;
	radius_rect->y -= radius/2;

}


void Player::Draw()
{
	SDL_RenderCopyEx(Game::gRenderer, img, NULL, rect, angle, &center, SDL_FLIP_NONE);
	if (debug)
	{
		SDL_RenderCopy(Game::gRenderer, rad_img, NULL, radius_rect);
	}
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

	if (abs(to_rotate) >= MAX_ANGLE_MODIFIER)
	{
		if (to_rotate > 0) { to_rotate = MAX_ANGLE_MODIFIER; }
		if (to_rotate < 0) { to_rotate = -(MAX_ANGLE_MODIFIER); }
	}

	angle += to_rotate * dt;
	angle = angle % 360;

	pos_x += vel_x * dt;
	pos_y += vel_y * dt;

	rect->x = pos_x;
	rect->y = pos_y;
	if (debug)
	{
		radius_rect->x = pos_x + center.x - radius / 2;
		radius_rect->y = pos_y + center.y - radius / 2;
	}

}

Player::~Player()
{
	std::cout << "Deleted Player Object" << std::endl;
	delete rect;
	rect = nullptr;
}