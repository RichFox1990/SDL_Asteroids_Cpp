#include "StateIntro.h"

#include <stdio.h>

void StateIntro::Init()
{
	printf("Initialising...\n");
}

void StateIntro::Cleanup()
{
	printf("Cleaning up..\n");
}

void StateIntro::Pause()
{
	printf("Intro State Paused.\n");
}

void StateIntro::Resume()
{
	printf("Intro State Resumed\n");
}

void StateIntro::HandleEvents(StateMachine* sm)
{
	// Handle events
}

void StateIntro::Update(StateMachine* sm)
{
	// Update
}

void StateIntro::Draw(StateMachine* sm)
{
	// Draw
}
