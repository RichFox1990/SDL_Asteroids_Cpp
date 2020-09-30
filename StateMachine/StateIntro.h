#pragma once
#include "GameState.h"

class DelayTimer;

class StateIntro : public GameState
{
public:
    void Init(Game* game) override;
    void Cleanup() override;

    void Pause() override;
    void Resume() override;

    void HandleEvents(StateMachine* sm, Game* game) override;
    void Update(StateMachine* sm, Game* game) override;
    void Draw(StateMachine* sm, Game* game) override;

private:
    DelayTimer* intro_timer;
};

