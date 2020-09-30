#include "StateWaveTransition.h"
#include "StatePlaying.h"

#include <stdio.h>
#include "SDL.h"
#include "..\Game.h"

void StateWaveTransition::Init(Game* game)
{
	printf("Initialising Wave Transition State\n");
}

void StateWaveTransition::Cleanup()
{
	printf("Cleaning up Wave Transition..\n");
	
}

void StateWaveTransition::Pause()
{
	printf("Wave Transition State Paused.\n");
}

void StateWaveTransition::Resume()
{
	printf("Wave Transition State Resumed\n");
}

void StateWaveTransition::HandleEvents(StateMachine* sm, Game* game)
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

		else if (events.type == SDL_KEYUP)
		{
			
		}
	}
}

void StateWaveTransition::Update(StateMachine* sm, Game* game)
{
	
}

void StateWaveTransition::Draw(StateMachine* sm, Game* game)
{
	//Clear
	SDL_SetRenderDrawColor(game->gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(game->gRenderer);

	// Draw

	//Update screen
	SDL_RenderPresent(game->gRenderer);
}
