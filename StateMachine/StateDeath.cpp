#include "StateDeath.h"
#include "StatePlaying.h"

#include <stdio.h>
#include "SDL.h"
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "..\Game.h"
#include "..\Player.h"
#include "..\Asteroid.h"
#include "..\Bullet.h"
#include "..\Particles.h"

void StateDeath::Init(Game* game)
{
	printf("Initialising Death State\n");

	game->vec_power_ups.clear();
	game->player->to_rotate = 2500;
	game->player->setImage(Game::game_images[Game::eImages::SHIP]);
	Mix_HaltChannel(Game::eSounds::SHIP_THRUST);
	Mix_PlayChannel(Game::eSounds::DEATH, Game::game_sounds[Game::eSounds::DEATH], 0);
	// TODO move this into own state after fleshing out/ redoing the highscore functionality
	std::cout << "The 3rd highest score is " << game->high_score.top3[2].second << std::endl;

	if (game->score > game->high_score.top3[2].second)
	{
		Mix_HaltChannel(-1);
		Mix_PlayChannel(Game::eSounds::HIGH_SCORE, Game::game_sounds[Game::eSounds::HIGH_SCORE], 0);
		std::string name;
		std::cout << "\nYou scored in the top 3. Please type your name(no spaces): " << std::endl;
		std::cin >> name;
		std::cout << std::endl;
		game->high_score.PushScore(name, game->score);
		game->high_score.Print();
	}
	else
	{
		std::cout << "\nYou didn't score in the top 3. Better luck next time\n" << std::endl;
	}
	death1_rect = new SDL_Rect;
	play_rect = new SDL_Rect;
	score_rect = new SDL_Rect;

	death1 = game->LoadRenderedText(death1, "YOU DIED", game->gtext_color, game->l_font, *death1_rect, game->s_r);
	play_again = game->LoadRenderedText(play_again, "Play again? (y/n)", game->gtext_color, game->g_font, *play_rect, game->s_r);
	death1_rect->x = game->SCREEN_WIDTH / 2 - death1_rect->w / 2;
	death1_rect->y = game->SCREEN_HEIGHT / 4 - death1_rect->h / 2;
	play_rect->x = game->SCREEN_WIDTH / 2 - play_rect->w / 2;
	play_rect->y = game->SCREEN_HEIGHT - game->SCREEN_HEIGHT / 4 - play_rect->h / 2;

	*score_rect = *death1_rect;
	score_rect->w /= 2;
	score_rect->h /= 2;
	score_rect->y = game->SCREEN_HEIGHT / 2 - score_rect->h / 2;
	score_rect->x = game->SCREEN_WIDTH / 2 - score_rect->w / 2;
}

void StateDeath::Cleanup()
{
	printf("Cleaning up Death state..\n");

	SDL_DestroyTexture(death1);
	SDL_DestroyTexture(play_again);
	SDL_DestroyTexture(gScore);
	death1 = play_again = gScore = nullptr;

	delete death1_rect;
	delete play_rect;
	delete score_rect;
	death1_rect = play_rect = score_rect = nullptr;

	//delete this;
}

void StateDeath::Pause(Game* game)
{
	printf("Death State Paused.\n");
}

void StateDeath::Resume(Game* game)
{
	printf("Death State Resumed\n");
}

void StateDeath::HandleEvents(StateMachine* sm, Game* game)
{
	SDL_Event events = game->events;
	// Handle events
	while (SDL_PollEvent(&events) != 0)
	{
		if (events.type == SDL_QUIT)
		{
			std::cout << "exit" << std::endl;
			sm->Quit();
		}
		else if (events.type == SDL_KEYDOWN)
		{
			switch (events.key.keysym.sym)
			{
			case SDLK_y:
				sm->ResetGame(); // this also switches the game state to playing
				break;
			case SDLK_n:
				sm->Quit();
				break;
			}
		}
	}
}

void StateDeath::Update(StateMachine* sm, Game* game)
{	
	float dt = sm->GetDelta();

	for (auto& ast : game->vec_bg_asteroids)
	{
		ast->Update(dt);
		ast->WrapCoords(game->SCREEN_WIDTH, game->SCREEN_HEIGHT);
	}

	if (game->vec_asteroids.size() != 0)
	{
		for (auto& ast : game->vec_asteroids)
		{
			//if (ast.get() != game->player->asteroid_death->get())
			ast->Update(dt);
			ast->WrapCoords(game->SCREEN_WIDTH, game->SCREEN_HEIGHT);
		}
	}

	if (game->vec_particles.size() != 0)
	{
		for (auto& par : game->vec_particles)
		{
			par->Update(dt);
		}
	}

	game->player->Update(dt);
	if (abs(game->player->vel_x) > 0.1 || abs(game->player->vel_y) > 0.1)
	{
		game->player->vel_x *= 0.995;
		game->player->vel_y *= 0.995;
	}
	else
	{
		game->player->vel_x = 0;
		game->player->vel_y = 0;
	}
	game->player->WrapCoords(game->SCREEN_WIDTH, game->SCREEN_HEIGHT);

	if (game->particle_delay.DelayComplete(true))
	{
		for (int i = 0; i < 3; i++)
		{
			float radians = 0.0f + i * (PI * 2 / 4.0f);
			game->vec_particles.push_back(std::make_unique<Particles>(game->player->pos_x + game->player->center.x, game->player->pos_y + game->player->center.y, radians, 125, 360, 10, game->s_r));
		}
	}

	game->vec_bullets.erase(std::remove_if(game->vec_bullets.begin(), game->vec_bullets.end(), [](std::unique_ptr<Entity>& e) { return (e->to_remove); }), game->vec_bullets.end());
	//game->vec_asteroids.erase(std::remove_if(game->vec_asteroids.begin(), game->vec_asteroids.end(), [](std::unique_ptr<Entity>& e) { return (e->is_dead); }), game->vec_asteroids.end());
	game->vec_particles.erase(std::remove_if(game->vec_particles.begin(), game->vec_particles.end(), [](std::unique_ptr<Particles>& e) { return (e->is_complete); }), game->vec_particles.end());
}

void StateDeath::Draw(StateMachine* sm, Game* game)
{
	//Clear
	SDL_RenderClear(game->gRenderer);
	SDL_RenderCopy(Game::gRenderer, Game::game_images[Game::eImages::BACKGROUND], NULL, NULL);


	// Draw
	for (auto& ast : game->vec_bg_asteroids)
	{
		ast->Draw();
	}
	if (game->vec_asteroids.size() != 0)
	{
		for (auto& ast : game->vec_asteroids)
		{
			ast->Draw();
		}
	}
	if (game->vec_particles.size() != 0)
	{
		for (auto& par : game->vec_particles)
		{
			par->Draw();
		}
	}

	game->player->Draw();
	SDL_RenderCopy(game->gRenderer, game->gScore, NULL, score_rect);
	SDL_RenderCopy(game->gRenderer, death1, NULL, death1_rect);
	SDL_RenderCopy(game->gRenderer, play_again, NULL, play_rect);

	//Update screen
	SDL_RenderPresent(game->gRenderer);
}
