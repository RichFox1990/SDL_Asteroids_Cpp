#pragma once
#include "GameState.h"

struct SDL_Texture;
struct SDL_Rect;

class StateDeath : public GameState
{
public:
    void Init(Game* game) override;
    void Cleanup() override;

    void Pause(Game* game) override;
    void Resume(Game* game) override;

    void HandleEvents(StateMachine* sm, Game* game) override;
    void Update(StateMachine* sm, Game* game) override;
    void Draw(StateMachine* sm, Game* game) override;

private:
    SDL_Texture* gScore = nullptr;
    SDL_Texture* death1 = nullptr;
    SDL_Texture* play_again = nullptr;

    SDL_Rect* death1_rect = nullptr;
    SDL_Rect* play_rect = nullptr;
    SDL_Rect* score_rect = nullptr;
};

