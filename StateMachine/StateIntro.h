#pragma once
#include "GameState.h"

class StateIntro : public GameState
{
public:
    void Init() override;
    void Cleanup() override;

    void Pause() override;
    void Resume() override;

    void HandleEvents(StateMachine* sm) override;
    void Update(StateMachine* sm) override;
    void Draw(StateMachine* sm) override;
};

