#include "PowerUp.h"
#include "Game.h"


PowerUp::PowerUp(const Entity* const ast, const float screen_ratio)
{
	MAX_SPEED = 175.0 * screen_ratio;
	img = Game::game_images[Game::eImages::SHEILD_PICKUP];

	rect = Game::GetRect(img, 0, 0);
	rect->w /= 7.5;
	rect->h /= 7.5;
	rect->w *= screen_ratio;
	rect->h *= screen_ratio;
	width = rect->w;
	height = rect->h;

	orig_size = std::make_pair(rect->w, rect->h);

	radius = (float)rect->w * 0.5;

	// unsure if needed yet
	center.x = rect->w * .5;
	center.y = rect->h * .5;
	pos_x = rect->x = (ast->pos_x + ast->center.x) - center.x;
	pos_y = rect->y = (ast->pos_y + ast->center.y) - center.y;
	this->vel_x = ast->vel_x * 0.7f;
	this->vel_y = ast->vel_y * 0.7f;
}

void PowerUp::HandleDamage()
{
	if (life_time.DelayProgress() > 0.6f || progress_offset != 0)
	{
		to_remove = true;
	}
	else
	{
		progress_offset = 0.6f;
	}
}



void PowerUp::Update(const float dt)
{
	pulse.DelayComplete(true);

	pos_x += pulse_offset.first;
	pos_y += pulse_offset.second;

	pos_x += vel_x * dt;
	pos_y += vel_y * dt;

	float progress = pulse.DelayProgress();

	if (progress < .5f)
	{
		rect->w = orig_size.first + ((orig_size.first * 0.3f) * progress*2);
		rect->h = orig_size.second + ((orig_size.second * 0.3f) * progress*2);

		pulse_offset.first = (rect->w - orig_size.first) * 0.5f;
		pulse_offset.second = (rect->h - orig_size.second) * 0.5f;
	}
	else
	{
		progress -= 0.5f;
		rect->w = orig_size.first + ((orig_size.first * 0.3f) * (1.0f- progress*2));
		rect->h = orig_size.second + ((orig_size.second * 0.3f) * (1.0f- progress*2));

		pulse_offset.first = (rect->w - orig_size.first) * 0.5f;
		pulse_offset.second = (rect->h - orig_size.second) * 0.5f;
	}
	width = rect->w;
	height = rect->h;

	pos_x -= pulse_offset.first;
	pos_y -= pulse_offset.second;

	rect->x = pos_x;
	rect->y = pos_y;

	if (life_time.DelayProgress() + progress_offset > 0.6f)
	{
		if (flash.DelayComplete(true))
		{
			draw = !draw;
		}
	}
	if (life_time.DelayComplete(false) || life_time.DelayProgress() + progress_offset >= 1)
	{
		to_remove = true;
	}
}


void PowerUp::Draw() const
{
	if (draw)
	{
		SDL_RenderCopy(Game::gRenderer, img, NULL, rect);
	}
}


PowerUp::~PowerUp()
{
	img = nullptr;
	delete rect;
	rect = nullptr;
}