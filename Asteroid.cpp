#include "Asteroid.h"
#include "Game.h"
//#include "stdlib.h"

Asteroid::Asteroid(const float x, const float y, const float vx, const float vy, const int angle, float size, const bool isCollidable, const float s_r)
{
	MAX_SPEED = 350.0 * s_r;
	//set position and velocity
	pos_x = x;
	pos_y = y;
	vel_x = vx;
	vel_y = vy;

	// set is the asteroid is just background object, or actually collidable (able to shoot it etc)
	this->isCollidable = isCollidable;

	//set starting angle of the image rotation
	this->angle = angle;

	// set to image 1 or 2 depending n the random number given.
	if (!this->isCollidable) // If its a background asteroid - use more basic image
	{
		img = Game::game_images[Game::eImages::BG_ASTEROID];
		rect = Game::GetRect(img, x, y);
	}
	else
	{
		// random number to pic between 2 asteroid images
		float rand_img = ((float)rand() / (float)RAND_MAX);
		if (rand_img <= .5) // image 1
		{
			img = Game::game_images[Game::eImages::ASTEROID1];
			rect = Game::GetRect(img, x, y);
		}
		else // image 2
		{
			img = Game::game_images[Game::eImages::ASTEROID2];
			rect = Game::GetRect(img, x, y);
		}
	}

	// scale 320x320 image down to 32x32
	rect->w /= 10;
	rect->h /= 10;
	// scale by original game size / captured screen ratio
	rect->w *= s_r;
	rect->h *= s_r;

	// scale asteroid depending on variable
	if (isCollidable) // If its not just for the background
	{
		this->size = size;
		rect->w *= size * s_r;
		rect->h *= size * s_r;
		angle_modifier = (((float)rand() / (float)RAND_MAX) * 200.0) + 20.0;
	}
	else // if its background asteroid
	{
		this->size = 10 * s_r;// (5 * Asteroid::SMALL) / s_r; //(size/ s_r) + ((10 - Asteroid::SMALL) / s_r);
		rect->w = 5 * s_r;//this->size;
		rect->h = 5 * s_r;//this->size;
		angle_modifier = 0; // no need to waste resources altering angle each frame for tiny background entities
	}
	// set width/height variables
	width = rect->w;
	height = rect->h;
	// set radius (for collisions) 
	radius = ((float)rect->h * .5);
	center.x = rect->w * .5;
	center.y = rect->h * .5;

	if (isCollidable)
	{
		rad_img = Game::game_images[Game::eImages::CIRCLE];
		radius_rect = Game::GetRect(rad_img, pos_x + center.x, pos_y + center.y);
		radius_rect->w = radius * 2;
		radius_rect->h = radius * 2;
		radius_rect->x -= radius;
		radius_rect->y -= radius;
	}
}


void Asteroid::Update(const float dt)
{
	pos_x += vel_x * dt;
	pos_y += vel_y * dt;

	rect->x = pos_x;
	rect->y = pos_y;

	if (isCollidable)
	{
		angle += angle_modifier * dt;
		angle = angle % 360;

		if (debug)
		{
			radius_rect->x = pos_x + center.x - radius;
			radius_rect->y = pos_y + center.y - radius;
		}
	}
}


void Asteroid::Draw() const
{

	if (isCollidable)
	{
		SDL_SetTextureColorMod(img, 255, 255, 255);
		SDL_RenderCopyEx(Game::gRenderer, img, NULL, rect, angle, &center, SDL_FLIP_NONE);
		if (debug)
		{
			SDL_RenderCopy(Game::gRenderer, rad_img, NULL, radius_rect);
		}
	}
	else
	{
		if (draw)
		{
			SDL_SetTextureColorMod(img, 255, 255, 255);
			SDL_RenderCopy(Game::gRenderer, img, NULL, rect);
		}
	}
}


Asteroid::~Asteroid()
{
	//std::cout << "Deleted Asteroid Object" << std::endl;

	//SDL_DestroyTexture(img);
	img = nullptr;
	delete rect;
	rect = nullptr;
}