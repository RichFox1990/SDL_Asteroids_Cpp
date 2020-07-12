#include "Player.h"
#include "Game.h"
//#include "TextureManager.h"

Player::Player(double x, double y)
{
	pos_x = x;
	pos_y = y;

	img = Game::game_images[Game::eImages::SHIP];
	rect = Game::GetRect(img, x, y);

	radius = (rect->h * .5) *.9;
	width = rect->w;
	height = rect->h;

	center.x = rect->w * .5;
	center.y = rect->h * .5;

	//rad_img = Game::game_images[Game::eImages::CIRCLE];
	//radius_rect = Game::GetRect(rad_img, pos_x, pos_y);
	//double orig_w = radius_rect->w;

	// scale the image (original is 100x100)
	//radius_rect->w *= ((double)width / orig_w)*.9;
	//radius_rect->h *= ((double)width / orig_w)*.9;

	//center the circle
	//radius_rect->x += center.x - radius_rect->w / 2;
	//radius_rect->y += center.y - radius_rect->h / 2;
}


void Player::Draw()
{
	//SDL_SetTextureColorMod(rad_img, 255, 255, 0);
	//SDL_RenderCopy(Game::gRenderer, rad_img, NULL, radius_rect);
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

	//radius_rect->x = pos_x + center.x - radius_rect->w / 2;
	//radius_rect->y = pos_y + center.y - radius_rect->h / 2;
}

Player::~Player()
{
	std::cout << "Deleted Player Object" << std::endl;
	delete rect;
	rect = nullptr;
}