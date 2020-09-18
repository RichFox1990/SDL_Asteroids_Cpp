#include "StateMachine.h"
#include "GameState.h"
#include "StateIntro.h"
#include "StatePlaying.h"


void StateMachine::Init()
{
	// call game init/ setup functions
}

void StateMachine::Cleanup()
{
	// remove game elements if needed/ call game cleanup code
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
	states.back()->Init();
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
	states.back()->Init();
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
}

void StateMachine::HandleEvents()
{
	if (!states.empty())
	{
		states.back()->HandleEvents(this);
	}
}

void StateMachine::Update()
{
	if (!states.empty())
	{
		states.back()->Update(this);
	}
}

void StateMachine::Draw()
{
	if (!states.empty())
	{
		states.back()->Draw(this);
	}
}