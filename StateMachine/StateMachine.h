#pragma once

#include <vector>

class GameState;

class StateMachine
{
public:
	void Init();
	void Cleanup();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();

	bool Running() { return is_running; }
	void Quit() { is_running = false; }

private:
	// the stack of states
	std::vector<GameState*> states;

	bool is_running;
};