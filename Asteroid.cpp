#include "Asteroid.h"
#include "Game.h"
#include "stdlib.h"

Asteroid::Asteroid(double x, double y, double vx, double vy, int angle, int size, double rand_img, bool isCollidable)
{
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
	if (rand_img <= .5)
	{
		img = Game::game_images[Game::eImages::ASTEROID1];
		rect = Game::GetRect(img, x, y);
	}
	else
	{
		img = Game::game_images[Game::eImages::ASTEROID2];
		rect = Game::GetRect(img, x, y);
	}

	// scale asteroid depending on variable
	if (isCollidable) // If its not just for the background
	{
		rect->w *= size;
		rect->h *= size;
		angle_modifier = (((double)rand() / (double)RAND_MAX) * 200.0) + 20.0;
	}
	else // if its background asteroid
	{
		size += 2;
		rect->w /= size;
		rect->h /= size;
		angle_modifier = 0; // no need to waste resources altering angle each frame for tiny background entities
	}

	width = rect->w;
	height = rect->h;

	center.x = rect->w * .5;
	center.y = rect->h * .5;

	//angle_modifier = (((double)rand() / (double)RAND_MAX) * 200.0) + 20.0;
}


void Asteroid::Update(double const& dt)
{
	pos_x += vel_x * dt;
	pos_y += vel_y * dt;

	rect->x = pos_x;
	rect->y = pos_y;

	if (isCollidable)
	{
		angle += angle_modifier * dt;
		angle = angle % 360;
	}
}


void Asteroid::Draw()
{

	if (isCollidable)
	{
		SDL_SetTextureColorMod(img, 153, 153, 0);
		SDL_RenderCopyEx(Game::gRenderer, img, NULL, rect, angle, &center, SDL_FLIP_NONE);
	}
	else
	{
		SDL_SetTextureColorMod(img, 255, 255, 255);
		SDL_RenderCopy(Game::gRenderer, img, NULL, rect);
		
	}

}


Asteroid::~Asteroid()
{
	std::cout << "Deleted Asteroid Object" << std::endl;
	delete rect;
	rect = nullptr;
}