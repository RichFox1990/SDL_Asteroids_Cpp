#include "StateWaveComplete.h"
#include "StateWaveTransition.h"

//#include <stdio.h>
//#include <memory>
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "..\Game.h"
#include "..\Player.h"
#include"..\Bullet.h"
#include "..\BlackHole.h"
#include "..\Particles.h"
#include "..\Asteroid.h"
#include "..\DelayTimer.h"


void StateWaveComplete::Init(Game* game)
{
	printf("Initialising Wave Complete State\n");
	pGame = game;
	game->player->damaged = false;

	complete = std::make_unique<SDL_Rect>();

	wComplete = game->LoadRenderedText(wComplete, "WAVE " + std::to_string(game->current_level) + " COMPLETE", game->gtext_color, game->l_font, *complete, game->s_r);

	complete->w *= .6;
	complete->h *= .6;
	complete->x = game->SCREEN_WIDTH / 2 - complete->w / 2;
	complete->y = game->SCREEN_HEIGHT / 6 - complete->h / 2;

	//black_hole = std::make_unique<BlackHole>(game->SCREEN_WIDTH, game->SCREEN_HEIGHT, 1, game->s_r);
	black_hole = new BlackHole{ game->SCREEN_WIDTH, game->SCREEN_HEIGHT, 1, game->s_r };

	while (SDL_HasIntersection(black_hole->rect, game->player->rect) == SDL_TRUE)
	{
		black_hole->NewPosition(game->SCREEN_WIDTH, game->SCREEN_HEIGHT);
	}

	Mix_PlayChannel(Game::eSounds::BLACK_HOLE, Game::game_sounds[Game::eSounds::BLACK_HOLE], -1);

	this->sound_played = false;
	InterpPlayer = new DelayTimer{ 4000.0, false };
}

void StateWaveComplete::Cleanup()
{
	printf("Cleaning up Wave Complete..\n");

	SDL_DestroyTexture(wComplete);
	wComplete = nullptr;

	//delete complete;
	//complete = nullptr;

	delete black_hole;
	black_hole = nullptr;

	delete InterpPlayer;
	InterpPlayer = nullptr;

	delete copy_player_rect;
	copy_player_rect = nullptr;

	//TODO CHANGE ABOVE TO UNIQUE POINTERS
}

void StateWaveComplete::Pause(Game* game)
{
	printf("Wave Complete State Paused.\n");
}

void StateWaveComplete::Resume(Game* game)
{
	printf("Wave Complete State Resumed\n");
}

void StateWaveComplete::HandleEvents(StateMachine* sm, Game* game)
{
	if (game->player->Collision(*black_hole)) return;

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

void StateWaveComplete::Update(StateMachine* sm, Game* game)
{
	float dt = sm->GetDelta();
	
	black_hole->Update(dt);

	for (auto& ast : game->vec_bg_asteroids)
	{
		ast->Update(dt);
		ast->WrapCoords(game->SCREEN_WIDTH, game->SCREEN_HEIGHT);
		if (black_hole != nullptr)
		{
			if (ast->Collision(*black_hole))
			{
				ast->draw = false;
			}
			else
			{
				ast->draw = true;
			}
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
			pow->Update(dt);
			pow->WrapCoords(game->SCREEN_WIDTH, game->SCREEN_HEIGHT);

			if (pow->Collision(*(game->player)))
			{
				Mix_PlayChannel(Game::eSounds::COLLECT, Game::game_sounds[Game::eSounds::COLLECT], 0);
				pow->to_remove = true;
				// to do SWITCH STATEMENT
				sm->sheild_interp += game->player->Health(25.0f);
				game->vec_particles.push_back(std::make_unique<Particles>(pow.get(), game->s_r));

			}
			if (pow->Collision(*(black_hole)))
			{
				pow->to_remove = true;
			}
		}
	}
	sm->HandleSheildInterp();

	game->vec_power_ups.erase(std::remove_if(game->vec_power_ups.begin(), game->vec_power_ups.end(), [](std::unique_ptr<Entity>& e) { return (e->to_remove); }), game->vec_power_ups.end());

	if (!(game->player->Collision(*black_hole)))
	{
		game->player->Update(dt);
		game->player->WrapCoords(game->SCREEN_WIDTH, game->SCREEN_HEIGHT);

	}
	else
	{
		Mix_HaltChannel(Game::eSounds::SHIP_THRUST);
		entered_bh = true;
		black_hole->enlarge = true;
		game->player->vel_x = 0;
		game->player->vel_y = 0;

		if (!this->sound_played)
		{
			Mix_PlayChannel(Game::eSounds::ENTER_BH, Game::game_sounds[Game::eSounds::ENTER_BH], 0);
			//Mix_FadeOutChannel(Game::eSounds::ENTER_BH, 3750);

			// make a copy for when we return to gamePlaying state
			copy_player_rect = new SDL_Rect{ game->player->rect->x, game->player->rect->y, game->player->rect->w, game->player->rect->h };

			// ensure player has no left over velocity and is displaying standard ship image
			game->player->img = Game::game_images[Game::eImages::SHIP];

			interp_vec_y = ((pGame->SCREEN_HEIGHT / 2.0) - ((float)game->player->pos_y + game->player->center.y));
			interp_vec_x = ((pGame->SCREEN_WIDTH / 2.0) - ((float)game->player->pos_x + game->player->center.x));
			//interp_vec_y = ((black_hole->pos_y + (black_hole->center.y)) - (game->player->pos_y + game->player->center.y));
			//interp_vec_x = ((black_hole->pos_x + (black_hole->center.x)) - (game->player->pos_x + game->player->center.x));

			// reset timer we will use for interpulating the player to the center of the blackhole
			total_time = percentage_elapsed = 0.0;
			InterpPlayer->Reset();
			this->sound_played = true;
		}


		// distance from player center to blackhole center
		//double distance = sqrt((pow(vec_x, 2) + pow(vec_y, 2))); // needed?

	}
	if (entered_bh)
	{
		if (!InterpPlayer->DelayComplete(false)) // alter player based on time passed
		{
			// for readability
			percentage_elapsed = InterpPlayer->DelayProgress();
			float orig_w = copy_player_rect->w;
			float orig_h = copy_player_rect->h;

			// reduce rect size over time
			game->player->rect->w = orig_w * (1.0 - percentage_elapsed);
			game->player->rect->h = orig_h * (1.0 - percentage_elapsed);

			// alter center point to ensure its rotated on correct axis
			game->player->center.x = game->player->rect->w / 2.0;
			game->player->center.y = game->player->rect->h / 2.0;

			// alter position over time
			game->player->pos_x = copy_player_rect->x + (interp_vec_x * percentage_elapsed);
			game->player->pos_y = copy_player_rect->y + (interp_vec_y * percentage_elapsed);

			// rotate the player based on orientation to blackhole and the new angle
			interp_vec_x >= 0 ? game->player->to_rotate -= 1 : game->player->to_rotate += 1;
			game->player->angle += game->player->to_rotate * (dt * 1000.0);
			game->player->angle = game->player->angle % 360;

			black_hole->InterpCentre(dt , InterpPlayer->delay / 1000.0);

			game->player->Update(dt);
		}
		else if (!popped_state) // change state
		{
			popped_state = true;

			//setup next stage
			Mix_HaltChannel(Game::eSounds::BLACK_HOLE);
			Mix_HaltChannel(Game::eSounds::ENTER_BH);
			sm->SetupNextWave();
			game->player->rect->w = copy_player_rect->w;
			game->player->rect->h = copy_player_rect->h;
			game->player->center.x = copy_player_rect->w / 2;
			game->player->center.y = copy_player_rect->h / 2;
			sm->PopState();
		}

		//SDL_RenderCopy(gRenderer, wComplete, NULL, &complete);
		//SDL_RenderPresent(gRenderer);
	}
}


void StateWaveComplete::Draw(StateMachine* sm, Game* game)
{
	//Clear
	SDL_RenderClear(Game::gRenderer);
	SDL_RenderCopy(Game::gRenderer, Game::game_images[Game::eImages::BACKGROUND], NULL, NULL);

	// Draw
	black_hole->Draw();

	for (auto& ast : game->vec_bg_asteroids)
	{
		ast->Draw();
	}
	if (game->vec_particles.size() != 0)
	{
		for (auto& par : game->vec_particles)
		{
			par->Draw();
		}
	}
	game->player->Draw();

	if (game->vec_power_ups.size() != 0)
	{
		for (auto& pow : game->vec_power_ups)
		{
			pow->Draw();
		}
	}
	//draw HUD
	// sheild
	SDL_RenderCopy(game->gRenderer, game->sheild_bg_bar, NULL, game->sheild_bar_rect);
	SDL_SetRenderDrawColor(game->gRenderer, 204, 255, 255, 255);
	SDL_RenderFillRect(game->gRenderer, game->sheild_amount_rect);
	SDL_RenderCopy(game->gRenderer, game->sheild_icon, NULL, game->sheild_icon_rect);

	SDL_RenderCopy(Game::gRenderer, wComplete, NULL, complete.get());

	//Update screen
	SDL_RenderPresent(Game::gRenderer);
}
