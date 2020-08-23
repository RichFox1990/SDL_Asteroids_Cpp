#include "PowerUp.h"
#include "Game.h"

PowerUp::PowerUp(const double ast_center_x, const double ast_center_y, const float screen_ratio)
{
	img = Game::game_images[Game::eImages::SHEILD];
	outer_img = Game::game_images[Game::eImages::OUTER_PICKUP];

	outer_rect = Game::GetRect(img, 0, 0);
	rect = Game::GetRect(img, 0, 0);
	rect->w /= 10;
	rect->h /= 10;
	outer_rect->w /= 10;
	outer_rect->h /= 10;
	rect->w *= screen_ratio;
	rect->h *= screen_ratio;
	outer_rect->w *= screen_ratio;
	outer_rect->h *= screen_ratio;

	// unsure if needed yet
	center.x = rect->w * .5;
	center.y = rect->h * .5;

	pos_x = rect->x = ast_center_x - center.x;
	pos_y = rect->y = ast_center_y - center.y;

	outer_rect->x = ast_center_x - outer_rect->w / 2.0;
	outer_rect->y = ast_center_y - outer_rect->h / 2.0;
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