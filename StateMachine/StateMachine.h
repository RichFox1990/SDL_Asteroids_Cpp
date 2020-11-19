#pragma once

#include <vector>
//#include "time.h"

class GameState;
class Game;
class Entity;

class StateMachine
{
public:
	Game* game = nullptr;
	StateMachine(Game* game);
	float sheild_interp = 0;

	void Init();
	void Cleanup();

	void ChangeState(GameState* state);
	void PushState(GameState* state);
	void PopState();

	void HandleEvents();
	void Update();
	void Draw();
	void Run();

	void CalculateDelta();
	void ResetDelta();
	float GetDelta() { return delta_time; };
	void HandleSheildInterp();
	void PowerUpChance(Entity* ent);

	bool Running() { return is_running; };
	void ResetGame() { reset_game = true; }
	void SetupNextWave();
	void Quit();

private:
	void FullResetGame();

	// the stack of states
	std::vector<GameState*> states;

	bool is_running = true;
	bool reset_game = false;

	// Delta time
	int FPS;
	float frame_delay;
	unsigned int frame_start = 0.0;
	unsigned int frame_time = 0.0;
	float delta_time = 0.0;
};