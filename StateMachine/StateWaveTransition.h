#pragma once
#include "GameState.h"
#include "..\DelayTimer.h"


class StateWaveTransition : public GameState
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
    //DelayTimer transition_timer{ 3500.0f, false };
};

