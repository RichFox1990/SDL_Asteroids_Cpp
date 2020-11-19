#pragma once
#include "StateMachine.h"


class GameState
{
public:
    virtual void Init(Game* game) = 0;
    virtual void Cleanup() = 0;

    virtual void Pause(Game* game) = 0;
    virtual void Resume(Game* game) = 0;

    virtual void HandleEvents(StateMachine* sm, Game* game) = 0;
    virtual void Update(StateMachine* sm, Game* game) = 0;
    virtual void Draw(StateMachine* sm, Game* game) = 0;

    void ChangeState(StateMachine* sm,
        GameState* state) {
        sm->ChangeState(state);
    }
};
