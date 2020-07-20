#include "Game.h"
#include "BlackHole.h"


BlackHole::BlackHole(const int screen_width, const int screen_height, const float size_multiplier, const float screen_ratio)
{
	debug = false;
	angle_modifier = 200;
	img = Game::game_images[Game::eImages::B_BLACK_HOLE];
	rect = Game::GetRect(img, 0, 0);

	rect->w *= screen_ratio;
	rect->h *= screen_ratio;
	pos_x = rect->x = (((double)rand() / (double)RAND_MAX)) * (screen_width - rect->w);
	pos_y = rect->y = (((double)rand() / (double)RAND_MAX)) * (screen_height - rect->h);

	rad_img = Game::game_images[Game::eImages::CIRCLE];
	radius_rect = Game::GetRect(rad_img, 0,0);

	radius = rect->w / 12;
	radius_rect->w = radius * 2;
	radius_rect->h = radius * 2;
	center.x = rect->w * .5;
	center.y = rect->h * .5;
	radius_rect->x = pos_x + center.x - radius_rect->w / 2;
	radius_rect->y = pos_y + center.y - radius_rect->h / 2;
}


void BlackHole::NewPosition(const int screen_width, const int screen_height)
{
	pos_x = rect->x = (((double)rand() / (double)RAND_MAX)) * (screen_width - rect->w);
	pos_y = rect->y = (((double)rand() / (double)RAND_MAX)) * (screen_height - rect->h);

	radius_rect->x = pos_x + center.x - radius_rect->w / 2;
	radius_rect->y = pos_y + center.y - radius_rect->h / 2;
}


void BlackHole::Update(const double dt)
{
	angle += angle_modifier * dt;
	angle = angle % 360;
}


void BlackHole::Draw() const
{
	SDL_RenderCopyEx(Game::gRenderer, img, NULL, rect, angle, &center, SDL_FLIP_NONE);
	if (debug)
	{
		SDL_RenderCopy(Game::gRenderer, rad_img, NULL, radius_rect);
	}

}


BlackHole::~BlackHole()
{
	//SDL_DestroyTexture(img);
	img = nullptr;
	delete rect;
	rect = nullptr;
}