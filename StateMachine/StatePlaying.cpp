#include "StatePlaying.h"

#include <stdio.h>

void StatePlaying::Init()
{
	printf("Initialising...\n");
}

void StatePlaying::Cleanup()
{
	printf("Cleaning up..\n");
}

void StatePlaying::Pause()
{
	printf("Playing State Paused.\n");
}

void StatePlaying::Resume()
{
	printf("Playing State Resumed\n");
}

void StatePlaying::HandleEvents(StateMachine* sm)
{
	// Handle events
}

void StatePlaying::Update(StateMachine* sm)
{
	// Update
}

void StatePlaying::Draw(StateMachine* sm)
{
	// Draw
}

