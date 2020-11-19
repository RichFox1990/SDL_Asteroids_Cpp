#include "Player.h"
#include "Game.h"
#include "Asteroid.h"

Player::Player(const float x, const float y, const float starting_sheild, const float s_r)
{
	current_sheild = starting_sheild;
	MAX_SPEED = 350.0 * s_r;
	pos_x = x;
	pos_y = y;

	img = Game::game_images[Game::eImages::SHIP];
	rect = Game::GetRect(img, x, y);
	rect->w /= 20 - (Asteroid::SMALL * 5);
	rect->h /= 20 - (Asteroid::SMALL * 5);
	rect->w *= s_r;
	rect->h *= s_r;

	radius = ((float)rect->w * .5) *.75;
	width = rect->w;
	height = rect->h;

	center.x = rect->w * .5;
	center.y = rect->h * .5;

	rad_img = Game::game_images[Game::eImages::CIRCLE];
	radius_rect = Game::GetRect(rad_img, pos_x + center.x, pos_y + center.y);
	radius_rect->w = radius*2;
	radius_rect->h = radius*2;
	radius_rect->x -= radius;
	radius_rect->y -= radius;

}


void Player::Draw() const
{
	if (damaged)
	{
		if (draw) 
		{ 
			// update loop has a timer that changes state of 'draw' while player is damaged to create a flashing effect when the player is hit
			SDL_RenderCopyEx(Game::gRenderer, img, NULL, rect, angle, &center, SDL_FLIP_NONE); 
		}
	}
	else
	{
		SDL_RenderCopyEx(Game::gRenderer, img, NULL, rect, angle, &center, SDL_FLIP_NONE);
	}

	if (debug)
	{
		SDL_RenderCopy(Game::gRenderer, rad_img, NULL, radius_rect);
	}
}


void Player::Update(const float dt)
{
	// Cap velocity and scale if limit reached
	float distance = sqrt((pow(vel_x, 2) + pow(vel_y, 2)));
	if (distance > MAX_SPEED)
	{
		vel_x = (vel_x / distance) * MAX_SPEED;
		vel_y = (vel_y / distance) * MAX_SPEED;
	}

	if (abs(to_rotate) > MAX_ANGLE_MODIFIER)
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

	// flashing effect timer for draw loop when ship is damaged
	if (flash.DelayComplete(true))
	{
		draw = !draw;
	}

	if (debug)
	{
		radius_rect->x = pos_x + center.x - radius;
		radius_rect->y = pos_y + center.y - radius;
	}
}

float Player::Health(const float amount)
{
	if (amount < 0)
	{
		if (current_sheild == 0)
		{
			to_remove = true;
			return 0.0f;
		}

		damaged = true;
	}
	else
	{
		if (current_sheild == 100)
		{
			// to do overdrive
			return 0.0f;
		}
	}
	
	// Alter sheild health
	float old_sheild = current_sheild;

	current_sheild += amount / 100;

	if (current_sheild > 1)
	{
		current_sheild = 1;
	}

	return current_sheild - old_sheild;
}

SDL_Texture* Player::getImage() const
{
	return img;
}

void Player::setImage(SDL_Texture* const new_img)
{
	img = new_img;
}

Player::~Player()
{
	std::cout << "Deleted Player Object" << std::endl;
	delete rect;
	rect = nullptr;
}