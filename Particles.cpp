#include "Particles.h"
#include "Game.h"


Particles::Particles(const int x, const int y, double radians, const double speed, const double offset, const float s_r)
{
	amount = 10;
	SPEED = speed;// *s_r;
	angle_modifier = radians;
	double vx = sin(radians) * SPEED;
	double vy = cos(radians) * SPEED;
	double orig_offset = offset;

	for (int i = 0; i < amount * 3; i++)
	{
		Pixel* new_pixel = new Pixel;
		new_pixel->rect = SDL_Rect{ x, y, 2, 2 };
		new_pixel->pos_x = x;
		new_pixel->pos_y = y;

		double new_offset = orig_offset * ((double)rand() / (double)RAND_MAX) - (offset / 2.0);
		float new_angle = angle_modifier + new_offset;
		float new_speed = SPEED/4.0f + (((float)rand() / (float)RAND_MAX) * (SPEED/2.0f));
		new_pixel->vel_x = new_speed * sinf(new_angle);
		new_pixel->vel_y = new_speed * cosf(new_angle);
		//new_pixel->vel_x = SPEED * cosf(((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f);
		//new_pixel->vel_y = SPEED * -sinf(((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f);

		pixels.push_back(new_pixel);
	}
}


Particles::Particles(Player* player, const int amount, const float s_r)
{
	double radians = player->angle * PI / 180.0f;
	double speed;
	//int x = player->pos_x + player->center.x;
	//int y = player->pos_y + player->center.y;
	int x = -sin((double)player->angle * PI / 180.0) * player->height / 4 + player->rect->x + player->width / 2;
	int y = cos((double)player->angle * PI / 180.0) * player->height / 4 + player->rect->y + player->height / 2;

	for (int i = 0; i < amount * 3; i++)
	{
		life_cycle.SetNewDelay(400.0f);

		Pixel* new_pixel = new Pixel;
		new_pixel->rect = SDL_Rect{ x, y, 2, 2 };
		new_pixel->pos_x = x;
		new_pixel->pos_y = y;

		// calculate an offset
		double offset = (((double)rand() / (double)RAND_MAX) * (PI)) - (PI/2.0f);
		float new_angle = radians + offset;

		speed = 75.0f + ((double)rand() / (double)RAND_MAX) * 25.0f;
		//float new_speed = player_speed / 2.0f + (((float)rand() / (float)RAND_MAX) * (player_speed));
		new_pixel->vel_x = speed * -sinf(new_angle);
		new_pixel->vel_y = speed * cosf(new_angle);
		//new_pixel->vel_x = SPEED * cosf(((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f);
		//new_pixel->vel_y = SPEED * -sinf(((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f);

		pixels.push_back(new_pixel);
	}
}



Particles::Particles(const int x, const int y, const float amount, const float s_r)
{
	pixels.reserve(amount*2);
	SPEED *= s_r;
	this->amount = amount;
	angle_modifier /= amount;
	float iteration;


	for (int i = 0; i < amount*3; i++)
	{
		iteration = (float)(i + 1);
		Pixel* new_pixel = new Pixel;
		new_pixel->rect = SDL_Rect{ x, y, 2, 2 };
		new_pixel->pos_x = x;
		new_pixel->pos_y = y;

		new_pixel->vel_x = ((float)rand() / (float)RAND_MAX)*SPEED * cosf(angle_modifier * iteration);
		new_pixel->vel_y = ((float)rand() / (float)RAND_MAX)*SPEED * sinf(angle_modifier * iteration);
		//new_pixel->vel_x = SPEED * cosf(((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f);
		//new_pixel->vel_y = SPEED * -sinf(((float)rand() / (float)RAND_MAX) * 2.0f * 3.14159f);

		pixels.push_back(new_pixel);
	}
}


void Particles::Update(double const dt)
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

	for (int i = 0, n = pixels.size(); i < n; i++)
	{
		if (i < n/3)
		{
			// yellow
			SDL_SetRenderDrawColor(Game::gRenderer, 255, 255, 0, opacity);
		}
		else if (i > n/3 && i < (n / 3) * 2)
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