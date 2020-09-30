#include "StateMachine.h"
#include "GameState.h"
#include "StateIntro.h"
#include "StatePlaying.h"
#include <stdio.h>
#include "..\Game.h"
#include "..\Player.h"
#include "..\Asteroid.h"

#include "time.h"


StateMachine::StateMachine(Game* game)
{
	this->game = game;
	this->FPS = game->FPS;
	frame_delay = 1000.0 / FPS;
}

void StateMachine::Init()
{
	// call game init/ setup functions
	printf("Initialising State Machine\n");

	states.reserve(5);
	this->ResetDelta();
	//this->ChangeState(state);
}

void StateMachine::Cleanup()
{
	// remove game elements if needed/ call game cleanup code
	while (!states.empty())
	{
		states.back()->Cleanup();

		delete states.back();
		states.back() = nullptr;

		states.pop_back();
	}
}

void StateMachine::CalculateDelta()
{
	frame_time = SDL_GetTicks() - frame_start;

	if (frame_time < frame_delay)
	{
		SDL_Delay(frame_delay - frame_time);
		delta_time = frame_delay / 1000.0;
	}
	else
	{
		delta_time = frame_time / 1000.0;
	}

	frame_start = SDL_GetTicks();
}

void StateMachine::ResetDelta()
{	
	delta_time = 0.0f;
	frame_time = 0.0f;
	frame_start = SDL_GetTicks();
}



void StateMachine::ChangeState(GameState* state)
{
	// Cleanup and delete the old state
	if (!states.empty())
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	// Add and Init the new state
	states.push_back(state);
	states.back()->Init(game);

	ResetDelta();
}

void StateMachine::PushState(GameState* state)
{
	// Pause the current state
	if (!states.empty())
	{
		states.back()->Pause();
	}

	// Add the new state and Init
	states.push_back(state);
	states.back()->Init(game);
}

void StateMachine::PopState()
{
	// Cleanup and remove current state
	if (!states.empty())
	{
		states.back()->Cleanup();
		states.pop_back();
	}

	// If theres still states in the list, Resume them.
	if (!states.empty())
	{
		states.back()->Resume();
	}
	else
	{
		printf("NOTHING TO RESUME!!!!\n");
	}
}

void StateMachine::SetupNextWave()
{
	// spawn new asteroids
	game->current_level++;
	game->MAX_ASTEROID_SPEED += 10;
	game->MIN_ASTEROID_SPEED += 5;
	double x, y;
	for (int i = 0; i < game->asteroid_amount + game->current_level; i++)
	{
		x = (((double)rand() / (double)RAND_MAX)) * game->SCREEN_WIDTH;
		y = (((double)rand() / (double)RAND_MAX)) * game->SCREEN_HEIGHT;
		game->CreateAsteroid(x, y, Asteroid::LARGE, true, false, game->vec_asteroids, game->s_r);
	}
}

void StateMachine::FullResetGame()
{
	srand((unsigned)time(NULL));

	game->MAX_ASTEROID_SPEED = 250 * game->s_r;
	game->MIN_ASTEROID_SPEED = 75 * game->s_r;

	//vc_b_asteroids.clear();
	game->vec_asteroids.clear();
	game->vec_bullets.clear();
	game->vec_particles.clear();
	game->vec_asteroids.reserve(50);
	game->vec_bullets.reserve(20);
	game->vec_particles.reserve(20);

	//player = std::make_unique<Player>(screen_center_x, screen_center_y, s_r);
	game->player->current_sheild = 1.0f;
	game->sheild_amount_rect->w = game->original_sheild_length * game->player->current_sheild;

	game->player->pos_x = game->screen_center_x;
	game->player->pos_y = game->screen_center_y;
	game->player->to_rotate = 0;
	game->player->is_dead = false;
	game->player->vel_x = game->player->vel_y = 0;

	game->score = 0;
	game->gScore = game->LoadRenderedText(game->gScore, "SCORE: " + std::to_string(game->score), game->gtext_color, game->g_font, game->score_rect, game->s_r);
	game->score_rect.x = game->SCREEN_WIDTH / 2 - game->score_rect.w / 2;
	game->score_rect.y = 10 * game->s_r;

	game->current_level = 1;
	// Create main asteroids
	double x, y;
	for (int i = 0; i < game->asteroid_amount + game->current_level; i++)
	{
		x = (((double)rand() / (double)RAND_MAX)) * game->SCREEN_WIDTH;
		y = (((double)rand() / (double)RAND_MAX)) * game->SCREEN_HEIGHT;
		game->CreateAsteroid(x, y, Asteroid::LARGE, true, false, game->vec_asteroids, game->s_r);
	}

	this->Cleanup();

	ChangeState(new StatePlaying);
}

void StateMachine::HandleEvents()
{
	if (!states.empty())
	{
		states.back()->HandleEvents(this, game);
	}
}

void StateMachine::Update()
{
	if (!states.empty())
	{
		states.back()->Update(this, game);
	}
}

void StateMachine::Draw()
{
	if (!states.empty())
	{
		states.back()->Draw(this, game);
	}
}

void StateMachine::Run()
{
	if (!reset_game)
	{
		this->HandleEvents();
		this->Update();
		this->Draw();
		this->CalculateDelta();
	}
	else
	{
		this->FullResetGame();
		reset_game = false;
	}
}

void StateMachine::Quit()
{
	is_running = false; 
	game->set_running(false);
}