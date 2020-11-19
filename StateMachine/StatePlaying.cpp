#include "StatePlaying.h"
#include "StateWaveComplete.h"
#include "StateWaveTransition.h"
#include "StateDeath.h"

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>

#include "..\Game.h"
#include "..\Player.h"
#include "..\Asteroid.h"
#include "..\Bullet.h"
#include "..\Particles.h"

void StatePlaying::Init(Game* game)
{
	printf("Initialising PLAYING STATE...\n");
	game->collision_delay.DelayComplete(true);
	game->player->damaged = true;
}

void StatePlaying::Cleanup()
{
	printf("Cleaning up..\n");
	//delete this;
}

void StatePlaying::Pause(Game* game)
{
	printf("Playing State Paused.\n");
}

void StatePlaying::Resume(Game* game)
{
	printf("Playing State Resumed\n");
	game->vec_power_ups.clear();
	game->collision_delay.DelayComplete(true);
	game->player->damaged = true;

	for (auto& ast : game->vec_bg_asteroids)
	{
		if (!(ast->draw))
		{
			ast->draw = true;
		}
	}
}

void StatePlaying::HandleEvents(StateMachine* sm, Game* game)
{
	float dt = sm->GetDelta();

	SDL_Event events = game->events;
	// Handle events
	while (SDL_PollEvent(&events) != 0)
	{
		if (events.type == SDL_QUIT)
		{
			std::cout << "exit PLAYING" << std::endl;
			sm->Quit();
		}

		else if (events.type == SDL_KEYDOWN)
		{
			switch (events.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				std::cout << "exit PLAYING" << std::endl;
				// MENU gamestate TODO
				sm->Quit();
				break;
			case SDLK_w:
				if ((game->player->getImage() != Game::game_images[Game::eImages::SHIP_THRUST] && (!game->player->to_remove)))
				{
					game->player->setImage(Game::game_images[Game::eImages::SHIP_THRUST]);
				}
				break;
			case SDLK_UP:
				if (game->player->getImage() != Game::game_images[Game::eImages::SHIP_THRUST])
				{
					game->player->setImage(Game::game_images[Game::eImages::SHIP_THRUST]);
				}
				break;
			case SDLK_i:
				game->player->debug = !(game->player->debug);
				if (!game->vec_asteroids.empty())
				{
					for (auto& e : game->vec_asteroids)
					{
						e->debug = game->player->debug;
					}
				}
				break;
			}
		}
		else if (events.type == SDL_KEYUP)
		{
			switch (events.key.keysym.sym)
			{
			case SDLK_w:
				game->player->setImage(Game::game_images[Game::eImages::SHIP]);
				Mix_HaltChannel(Game::eSounds::SHIP_THRUST);
				break;
			case SDLK_UP:
				game->player->setImage(Game::game_images[Game::eImages::SHIP]);
				Mix_HaltChannel(Game::eSounds::SHIP_THRUST);
				break;
			case SDLK_a:
				game->player->to_rotate = 0;
				break;
			case SDLK_LEFT:
				game->player->to_rotate = 0;
				break;
			case SDLK_d:
				game->player->to_rotate = 0;
				break;
			case SDLK_RIGHT:
				game->player->to_rotate = 0;
				break;
			}
		}
	}

	//Get the keystates
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	if (keystates[SDL_SCANCODE_W] || keystates[SDL_SCANCODE_UP])
	{
		game->player->vel_x += sin(game->player->angle * PI / 180) * game->SHIP_THRUST * dt;
		game->player->vel_y += -cos(game->player->angle * PI / 180) * game->SHIP_THRUST * dt;

		if (game->particle_delay.DelayComplete(true))
		{
			game->vec_particles.push_back(std::make_unique<Particles>(game->player.get(), 4, game->s_r));
		}

		if (!Mix_Playing(Game::eSounds::SHIP_THRUST))
		{
			Mix_PlayChannel(Game::eSounds::SHIP_THRUST, Game::game_sounds[Game::eSounds::SHIP_THRUST], 0);
		}
	}
	if (keystates[SDL_SCANCODE_A] || keystates[SDL_SCANCODE_LEFT])
	{
		game->player->to_rotate -= game->ANGLE_MODIFIER;
	}
	if (keystates[SDL_SCANCODE_D] || keystates[SDL_SCANCODE_RIGHT])
	{
		game->player->to_rotate += game->ANGLE_MODIFIER;
	}
	if (keystates[SDL_SCANCODE_LSHIFT] || keystates[SDL_SCANCODE_RSHIFT])
	{
		if (game->player->to_rotate != 0)
			game->player->to_rotate = game->player->to_rotate % (game->MAX_ANGLE_MOD * 5);
	}

	if (keystates[SDL_SCANCODE_SPACE] && (game->vec_asteroids.size() > 0))
	{
		if (game->shot_delay.DelayComplete(true))
		{
			game->vec_bullets.push_back(std::make_unique<Bullet>(game->player.get(), game->s_r));

			Mix_PlayChannel(Game::eSounds::SHOOT, Game::game_sounds[Game::eSounds::SHOOT], 0);

		}
	}
}

void StatePlaying::Update(StateMachine* sm, Game* game)
{
	// Update loop

	// delta time
	float dt = sm->GetDelta();

	// background asteroids
	if (game->vec_bg_asteroids.size() != 0)
	{
		for (auto& ast : game->vec_bg_asteroids)
		{
			ast->Update(dt);
			ast->WrapCoords(game->SCREEN_WIDTH, game->SCREEN_HEIGHT);
		}
	}
	// check collisions between player and asteroids
	if (game->vec_asteroids.size() != 0)
	{
		for (auto& ast : game->vec_asteroids)
		{
			// update and wrap coords on each asteroid
			ast->Update(dt);
			ast->WrapCoords(game->SCREEN_WIDTH, game->SCREEN_HEIGHT);

			if ((game->player->Collision(*ast)) && (game->collision_delay.DelayComplete(true)))
			{
				Mix_PlayChannel(Game::eSounds::SHIP_HIT, Game::game_sounds[Game::eSounds::SHIP_HIT], 0);
				game->score -= 500;
				if (game->score < 0) { game->score = 0; }
				// re render the HUD score texture
				game->gScore = game->LoadRenderedText(game->gScore, "SCORE: " + std::to_string(game->score), game->gtext_color, game->g_font, game->score_rect, game->s_r);

				sm->sheild_interp += game->player->Health(-25.0f);

				// collision particles (TODO better calculation)
				if (!(game->player->to_remove))
				{
					float radians = atan2(game->player->vel_x, game->player->vel_y);
					float previous_speed = sqrtf(powf(game->player->vel_x, 2) + powf(game->player->vel_y, 2));

					float offset = (PI) * ((float)rand() / (float)RAND_MAX);
					if (abs(offset) < .2) { offset = .2; }

					game->vec_particles.push_back(std::make_unique<Particles>(game->player.get(), game->s_r));
					//game->vec_particles.push_back(std::make_unique<Particles>(game->player->pos_x + game->player->center.x, game->player->pos_y + game->player->center.y, radians, previous_speed, offset, 15, game->s_r));
				}
			}
		}
	}
	// Update player and Wrap coords
	game->player->Update(dt);
	game->player->WrapCoords(game->SCREEN_WIDTH, game->SCREEN_HEIGHT);

	// if the player is currently damaged, monitor the collision delay timer and reset the 'damaged' bool to false when delay completed
	if (game->player->damaged)
	{
		if (game->collision_delay.DelayComplete(false))
		{
			game->player->damaged = false;
		}
	}


	if (game->vec_bullets.size() != 0)
	{
		// create temp vector to store any newly spawned asteroids (so we dont crash the vector we are iterating over)
		std::vector<std::unique_ptr<Entity>> vec_tempAsteroids;
		vec_tempAsteroids.reserve(2);

		//loop through bullets
		for (auto& bul : game->vec_bullets)
		{
			if (game->vec_power_ups.size() != 0)
			{
				for (auto& pow : game->vec_power_ups)
				{
					// if collision
					if (bul->Collision(*pow))
					{
						Mix_PlayChannel(Game::eSounds::HIT_POWER, Game::game_sounds[Game::eSounds::HIT_POWER], 0);
						bul->to_remove = true;
						dynamic_cast<PowerUp*>(pow.get())->HandleDamage();

						game->vec_particles.push_back(std::make_unique<Particles>(pow->pos_x + pow->center.x, pow->pos_y + pow->center.y, 10, game->s_r));
						break;
					}
				}
			}
			// for each bullet check collision with all main asteroids
			if (game->vec_asteroids.size() != 0)
			{
				for (auto& ast : game->vec_asteroids)
				{
					// if collision
					if (bul->Collision(*ast))
					{
						bul->to_remove = true;
						ast->to_remove = true;
						game->score += 100;
						game->gScore = game->LoadRenderedText(game->gScore, "SCORE: " + std::to_string(game->score), game->gtext_color, game->g_font, game->score_rect, game->s_r);
						if (ast->size > Asteroid::SMALL)
						{
							if (ast->size == Asteroid::MEDIUM)
							{
								Mix_PlayChannel(Game::eSounds::HIT_MED, Game::game_sounds[Game::eSounds::HIT_MED], 0);
							}
							else
							{
								Mix_PlayChannel(Game::eSounds::HIT_LRG, Game::game_sounds[Game::eSounds::HIT_LRG], 0);
							}

							game->Create2SubAsteroids((Asteroid*)(ast.get()), vec_tempAsteroids);
							break;
						}
						else
						{
							Mix_PlayChannel(Game::eSounds::HIT_SML, Game::game_sounds[Game::eSounds::HIT_SML], 0);

							game->vec_particles.push_back(std::make_unique<Particles>(ast->pos_x + ast->center.x, ast->pos_y + ast->center.y, 10, game->s_r));
							sm->PowerUpChance(ast.get());
						
							break;
						}

					}
				}
			}

			// update bullet location
			bul->Update(dt);
			// check if bullet is outside of screen (if so, set is_dead to true)
			bul->WrapCoords(game->SCREEN_WIDTH, game->SCREEN_HEIGHT);
		}

		// merge any new spawned asteroids into the main vector
		if (vec_tempAsteroids.size() != 0)
		{
			std::move(std::begin(vec_tempAsteroids), std::end(vec_tempAsteroids), std::back_inserter(game->vec_asteroids));
		}
	}

	if (game->vec_particles.size() != 0)
	{
		for (auto& par : game->vec_particles)
		{
			par->Update(dt);
		}
	}

	if (game->vec_power_ups.size() != 0)
	{
		for (auto& pow : game->vec_power_ups)
		{
			pow->WrapCoords(game->SCREEN_WIDTH, game->SCREEN_HEIGHT);
			pow->Update(dt);

			if (pow->Collision(*(game->player)))
			{
				Mix_PlayChannel(Game::eSounds::COLLECT, Game::game_sounds[Game::eSounds::COLLECT], 0);
				pow->to_remove = true;
				// to do SWITCH STATEMENT
				sm->sheild_interp += game->player->Health(25.0f);
				game->vec_particles.push_back(std::make_unique<Particles>(pow.get(), game->s_r));
			}
		}
	}

	sm->HandleSheildInterp();

	// If bullet/asteroid is marked as dead, remove them
	game->vec_bullets.erase(std::remove_if(game->vec_bullets.begin(), game->vec_bullets.end(), [](std::unique_ptr<Entity>& e) { return (e->to_remove); }), game->vec_bullets.end());
	game->vec_asteroids.erase(std::remove_if(game->vec_asteroids.begin(), game->vec_asteroids.end(), [](std::unique_ptr<Entity>& e) { return (e->to_remove); }), game->vec_asteroids.end());
	game->vec_particles.erase(std::remove_if(game->vec_particles.begin(), game->vec_particles.end(), [](std::unique_ptr<Particles>& e) { return (e->is_complete); }), game->vec_particles.end());
	game->vec_power_ups.erase(std::remove_if(game->vec_power_ups.begin(), game->vec_power_ups.end(), [](std::unique_ptr<Entity>& e) { return (e->to_remove); }), game->vec_power_ups.end());

	if (game->player->to_remove)
	{
		// change to death state
		ChangeState(sm, new StateDeath);
	}

	// IF KILLED ALL ASTEROIDS 
	// STATE WAVE COMPLETE ############################################################################
	if ((game->vec_asteroids.size() == 0) && (game->vec_bullets.empty()))
	{
		sm->PushState(new StateWaveComplete); // create state
	}
}

void StatePlaying::Draw(StateMachine* sm, Game* game)
{
	// Draw
	SDL_RenderClear(game->gRenderer);
	SDL_RenderCopy(Game::gRenderer, Game::game_images[Game::eImages::BACKGROUND], NULL, NULL);

	// draw background asteroids
	if (game->vec_bg_asteroids.size() != 0)
	{
		for (auto& ast : game->vec_bg_asteroids)
		{
			ast->Draw();
		}
	}
	// draw main asteroids
	if (game->vec_asteroids.size() != 0)
	{
		for (auto& ent : game->vec_asteroids)
		{
			ent->Draw();
		}
	}
	// explosions
	if (game->vec_particles.size() != 0)
	{
		for (auto& par : game->vec_particles)
		{
			par->Draw();
		}
	}
	// draw bullets
	if (game->vec_bullets.size() != 0)
	{
		for (auto& ent : game->vec_bullets)
		{
			ent->Draw();
		}
	}

	if (game->vec_power_ups.size() != 0)
	{
		for (auto& pow : game->vec_power_ups)
		{
			pow->Draw();
		}
	}

	//draw player
	game->player->Draw();

	//draw HUD
	// sheild
	SDL_RenderCopy(game->gRenderer, game->sheild_bg_bar, NULL, game->sheild_bar_rect);
	SDL_SetRenderDrawColor(game->gRenderer, 204, 255, 255, 255);
	SDL_RenderFillRect(game->gRenderer, game->sheild_amount_rect);
	SDL_RenderCopy(game->gRenderer, game->sheild_icon, NULL, game->sheild_icon_rect);
	// score
	SDL_RenderCopy(game->gRenderer, game->gScore, NULL, &(game->score_rect));

	//Update screen
	SDL_RenderPresent(game->gRenderer);
}

