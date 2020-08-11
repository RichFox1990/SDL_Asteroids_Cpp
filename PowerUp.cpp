#include "PowerUp.h"
#include "Game.h"

PowerUp::PowerUp(const double x, const double y, const float screen_ratio)
{
	img = Game::game_images[Game::eImages::SHEILD];
	outer_img = Game::game_images[Game::eImages::OUTER_PICKUP];

	outer_rect = Game::GetRect(img, 0, 0);
	rect = Game::GetRect(img, 0, 0);
	rect->w *= screen_ratio;
	rect->h *= screen_ratio;
	outer_rect->w *= screen_ratio;
	outer_rect->h *= screen_ratio;

	center.x = rect->w * .5;
	center.y = rect->h * .5;
}


void PowerUp::Update(const double dt)
{

}


void PowerUp::Draw() const
{
	SDL_RenderCopy(Game::gRenderer, img, NULL, rect);

	SDL_RenderCopy(Game::gRenderer, outer_img, NULL, outer_rect);
}


PowerUp::~PowerUp()
{

}