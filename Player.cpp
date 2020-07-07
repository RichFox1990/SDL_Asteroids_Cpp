#include "Player.h"
#include "Game.h"

Player::Player(double x, double y)
{
	pos.x = x;
	pos.y = y;
	is_dead = false;
	img = Game::game_images[Game::eImages::SHIP]->texture;
	rect = Game::game_images[Game::eImages::SHIP]->GetRect(x, y);

	center.x = rect->w * .5;
	center.y = rect->h * .6;
}


void Player::Draw()
{
	//SDL_RenderCopy(Game::gRenderer, img, NULL, rect);
	SDL_RenderCopyEx(Game::gRenderer, img, NULL, rect, angle, &center, SDL_FLIP_NONE);

}


void Player::Update(double const& dt)
{
	//if (vel.x > MAX_SPEED) { vel.x = MAX_SPEED; }
	//else if (vel.x < -MAX_SPEED) { vel.x = -MAX_SPEED; }

	//if (vel.y > MAX_SPEED) { vel.y = MAX_SPEED; }
	//else if (vel.y < -MAX_SPEED) { vel.y = -MAX_SPEED; }

	pos.x += vel.x * dt;
	pos.y += vel.y * dt;

	rect->x = pos.x;
	rect->y = pos.y;

	//std::cout << vel.x << " : " << vel.y << std::endl;
}

Player::~Player()
{
	std::cout << "Deleted Player Object" << std::endl;
	delete rect;
	rect = nullptr;
}