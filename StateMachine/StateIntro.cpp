#include "StateIntro.h"

#include <stdio.h>
#include "SDL.h"
#include "..\Game.h"
#include "StatePlaying.h"
#include "..\DelayTimer.h"

void StateIntro::Init(Game* game)
{
	printf("Initialising Intro State\n");
	intro_timer = new DelayTimer{ 3500.0f, false };
}

void StateIntro::Cleanup()
{
	printf("Cleaning up INTRO..\n");
	delete intro_timer;
	intro_timer = nullptr;
	//delete this;
}

void StateIntro::Pause(Game* game)
{
	printf("Intro State Paused.\n");
}

void StateIntro::Resume(Game* game)
{
	printf("Intro State Resumed\n");
}

void StateIntro::HandleEvents(StateMachine* sm, Game* game)
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
			ChangeState(sm, new StatePlaying);
		}
	}
}

void StateIntro::Update(StateMachine* sm, Game* game)
{
	if (intro_timer->DelayComplete(false))
	{
		printf("CHANGING TO PLAYING");
		ChangeState(sm, new StatePlaying);
	}
}

void StateIntro::Draw(StateMachine* sm, Game* game)
{
	//Clear
	SDL_SetRenderDrawColor(Game::gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(Game::gRenderer);

	// Draw
	SDL_RenderCopy(Game::gRenderer, game->game_images[Game::eImages::SPLASH], NULL, NULL);

	//Update screen
	SDL_RenderPresent(Game::gRenderer);
}
