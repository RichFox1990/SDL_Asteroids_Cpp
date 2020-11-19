#pragma once
#include "GameState.h"
class StatePlaying : public GameState
{
public:
    void Init(Game* game) override;
    void Cleanup() override;

    void Pause(Game* game) override;
    void Resume(Game* game) override;

    void HandleEvents(StateMachine* sm, Game* game) override;
    void Update(StateMachine* sm, Game* game) override;
    void Draw(StateMachine* sm, Game* game) override;
};

