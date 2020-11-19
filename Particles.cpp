#include "Particles.h"
#include "Game.h"
#include "Player.h"
#include <iostream>
#include <tuple>
#include <utility>

// larger ast explosion (directional and speed based)
Particles::Particles(const int x, const int y, float radians, const float speed, const float offset, const int amount, const float s_r)
{
	this->amount = amount;
	SPEED = speed;// *s_r;
	angle_modifier = radians;
	float vx = sin(radians) * SPEED;
	float vy = cos(radians) * SPEED;
	float orig_offset = offset;
	pixels.reserve(amount*3);


	for (int i = 0; i < amount * 3; i++)
	{
		pixels.push_back(std::make_unique<Pixel>());

		pixels.back()->rect = SDL_Rect{ x, y, 2, 2 };
		pixels.back()->pos_x = x;
		pixels.back()->pos_y = y;

		float new_offset = orig_offset * ((float)rand() / (float)RAND_MAX) - (offset / 2.0);
		float new_angle = angle_modifier + new_offset;
		float new_speed = SPEED/4.0f + (((float)rand() / (float)RAND_MAX) * (SPEED/2.0f));
		pixels.back()->vel_x = new_speed * sinf(new_angle);
		pixels.back()->vel_y = new_speed * cosf(new_angle);
	}
}

// Player thrust
Particles::Particles(Entity* player, const int amount, const float s_r)
{
	float radians = player->angle * PI / 180.0f;
	float speed;

	pixels.reserve(amount*3);

	int x = -sin((float)player->angle * PI / 180.0) * player->height / 4.0 + player->rect->x + player->width / 2.0;
	int y = cos((float)player->angle * PI / 180.0) * player->height / 4.0 + player->rect->y + player->height / 2.0;

	for (int i = 0; i < amount * 3; i++)
	{
		life_cycle.SetNewDelay(400.0f);

		pixels.push_back(std::make_unique<Pixel>());

		pixels.back()->rect = SDL_Rect{ x, y, 2, 2 };
		pixels.back()->pos_x = x;
		pixels.back()->pos_y = y;

		// calculate an offset
		float offset = (((float)rand() / (float)RAND_MAX) * (PI)) - (PI/2.0f);
		float new_angle = radians + offset;

		speed = 75.0f + ((float)rand() / (float)RAND_MAX) * 25.0f;
		pixels.back()->vel_x = speed * -sinf(new_angle);
		pixels.back()->vel_y = speed * cosf(new_angle);
	}
}


// small ast explosion
Particles::Particles(const int x, const int y, const float amount, const float s_r)
{
	SPEED *= s_r;
	this->amount = amount;
	angle_modifier /= amount;
	float iteration;
	pixels.reserve(amount*3);

	for (int i = 0; i < amount*3; i++)
	{
		iteration = (float)(i + 1);

		pixels.push_back(std::make_unique<Pixel>());

		pixels.back()->rect = SDL_Rect{ x, y, 2, 2 };
		pixels.back()->pos_x = x;
		pixels.back()->pos_y = y;

		pixels.back()->vel_x = ((float)rand() / (float)RAND_MAX)*(SPEED*s_r) * cosf(angle_modifier * iteration);
		pixels.back()->vel_y = ((float)rand() / (float)RAND_MAX)*(SPEED*s_r) * sinf(angle_modifier * iteration);
	}
}

// Player Hit
Particles::Particles(Entity* entity, const float s_r)
{
	float speed;
	int pixel_size = 0;
	float life_time = 300.0f;
	int rAngle = 0;

	if (dynamic_cast<Player*>(entity) != nullptr)
	{
		ent_type = 'p';
		pixel_size = 5;
		rAngle = entity->angle;
	}
	else if (dynamic_cast<PowerUp*>(entity) != nullptr)
	{
		ent_type = 'c';
		pixel_size = 3;
		life_time /= 3.0;
		rAngle = entity->angle;
	}
	else
	{
		std::cout << "UNKOWN ENTITY" << std::endl;
	}

	amount = 40;
	float radians = rAngle * PI / 180.0f;
	float offset = 0;

	pixels.reserve(amount * 4);

	for (int i = 0; i < 4; i++)
	{
		int x = entity->rect->x + entity->width / 2.0;
		int y = entity->rect->y + entity->height / 2.0;
		rAngle += i * 90.0;
		rAngle = rAngle % 360;
		float radians = rAngle * PI / 180.0f;

		for (int j = 0; j < amount; j++)
		{
			life_cycle.SetNewDelay(life_time);

			pixels.push_back(std::make_unique<Pixel>());

			pixels.back()->rect = SDL_Rect{ x, y, pixel_size, pixel_size };
			pixels.back()->pos_x = x;
			pixels.back()->pos_y = y;

			// calculate an offset
			
			offset = (((float)rand() / (float)RAND_MAX) * (PI / 2.0));
			float new_angle = radians +offset;
			if (j < amount / 2.0)
			{
				speed = 400.0f * s_r;
			}
			else
			{
				speed = (400.0f - ((float)rand() / (float)RAND_MAX) * 100.0f) * s_r;
			}
			
			pixels.back()->vel_x = speed * -sinf(new_angle);
			pixels.back()->vel_y = speed * cosf(new_angle);
		}
	}
}


void Particles::Update(float const dt)
{
	delta_time = dt;

	for (int i = 0; i < pixels.size(); i++)
	{
		pixels[i]->pos_x += pixels[i]->vel_x * dt;
		pixels[i]->pos_y += pixels[i]->vel_y * dt;
		pixels[i]->rect.x = pixels[i]->pos_x;
		pixels[i]->rect.y = pixels[i]->pos_y;
	}

	if (life_cycle.DelayComplete(true))
	{
		is_complete = true;
	}
}


void Particles::Draw() const
{
	SDL_SetRenderDrawBlendMode(Game::gRenderer, SDL_BLENDMODE_BLEND);

	time_elapsed -= (delta_time * 1000) / life_cycle.delay;

	opacity = 255.0 * (time_elapsed);
	if (opacity < 0)
	{
		opacity = 0;
	}

	if (ent_type == NULL)
	{
		for (int i = 0, n = pixels.size(); i < n; i++)
		{
			if (i < n / 3)
			{
				// yellow
				SDL_SetRenderDrawColor(Game::gRenderer, 255, 255, 0, opacity);
			}
			else if (i > n / 3 && i < (n / 3) * 2)
			{
				// orange
				SDL_SetRenderDrawColor(Game::gRenderer, 255, 165, 0, opacity);
			}
			else if (i > (n / 3) * 2)
			{
				// red
				SDL_SetRenderDrawColor(Game::gRenderer, 255, 0, 0, opacity);
			}
			else
			{
				// white
				SDL_SetRenderDrawColor(Game::gRenderer, 255, 255, 255, opacity);
			}

			SDL_RenderFillRect(Game::gRenderer, &(pixels[i]->rect));
		}
	}
	else if (ent_type == 'p')
	{
		// white
		for (int i = 0, n = pixels.size(); i < n; i++)
		{
			SDL_SetRenderDrawColor(Game::gRenderer, 255, 255, 255, opacity);
			SDL_RenderFillRect(Game::gRenderer, &(pixels[i]->rect));
		}
	}
	else
	{
		// green
		for (int i = 0, n = pixels.size(); i < n; i++)
		{
			SDL_SetRenderDrawColor(Game::gRenderer, 152, 251, 152, opacity);
			SDL_RenderFillRect(Game::gRenderer, &(pixels[i]->rect));
		}
	}
}