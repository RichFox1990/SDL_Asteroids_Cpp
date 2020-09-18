#pragma once
#include "StateMachine.h"

class GameState
{
public:
    virtual void Init() = 0;
    virtual void Cleanup() = 0;

    virtual void Pause() = 0;
    virtual void Resume() = 0;

    virtual void HandleEvents(StateMachine* game) = 0;
    virtual void Update(StateMachine* game) = 0;
    virtual void Draw(StateMachine* game) = 0;

    void ChangeState(StateMachine* game,
        GameState* state) {
        game->ChangeState(state);
    }
};
