#pragma once
#include "GameState.h"
#include <stdio.h>
#include <memory>

struct SDL_Texture;
struct SDL_Rect;
class BlackHole;
class DelayTimer;

class StateWaveComplete : public GameState
{
public:
    void Init(Game* game) override;
    void Cleanup() override;

    void Pause() override;
    void Resume() override;

    void HandleEvents(StateMachine* sm, Game* game) override;
    void Update(StateMachine* sm, Game* game) override;
    void Draw(StateMachine* sm, Game* game) override;

    //std::unique_ptr<BlackHole> black_hole = nullptr;
    BlackHole* black_hole = nullptr;
    std::unique_ptr<SDL_Rect> complete = nullptr;
    SDL_Texture* wComplete = nullptr;
    SDL_Texture* lComplete = nullptr;

    //std::unique_ptr<DelayTimer> InterpPlayer;
    DelayTimer* InterpPlayer = nullptr;
    double percentage_elapsed = 0.0;
    double total_time = 0.0;
    bool sound_played = false;
    bool popped_state = false;

    double interp_vec_x, interp_vec_y;
    SDL_Rect* copy_player_rect = nullptr;
};

