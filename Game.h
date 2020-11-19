#pragma once
#include <iostream>
#include <string>
#include "SDL.h"
#include <vector>
#include <array>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "Particles.h"
#include "PowerUp.h"

// TODO refactor below to reference and pointer within code
//#include "DelayTimer.h"
#include "HighScore.h"

// forward decloration
class Entity;
class Player;
class DelayTimer;
class Asteroid;
//class Particles;

class StateMachine;
class StatePlaying;
class StateDeath;
class StateWaveTransition;
class StateWaveComplete;

static const double PI = 3.14159265;

class Game {
public:
	Game(const int FPS);
	~Game();

	friend StateMachine;
	friend StatePlaying;
	friend StateDeath;
	friend StateWaveTransition;
	friend StateWaveComplete;
	
	// Methods
	static SDL_Rect* GetRect(SDL_Texture* const texture, const int x, const int y);
	static SDL_Event events;
	static SDL_Renderer* gRenderer;
	// State machine for the game
	std::unique_ptr<StateMachine> state_machine;

	HighScore high_score{};
	//Globally used font
	TTF_Font* g_font = nullptr;
	TTF_Font* l_font = nullptr;

	//void SplashScreen(const int time_to_display) const;
	//void HandleInput();
	//void Update();
	//void Draw() const;
	//void Render() const;
	void Run();

	bool is_running() const;
	void set_running(const bool new_bool);

	float starting_sheild = 0.5f;
	float drop_rate = 0.1;

	bool wave_complete = false;
	mutable bool game_reset = false;
	//char play_again = NULL;
	int FPS;
	SDL_Window* gWindow = nullptr;


	struct GameState
	{
		enum states
		{
			INIT,
			CREATE_ENTITIES,
			MAIN_MENU,
			LEVEL_PLAY,
			LEVEL_COMPLETE,
			LEVEL_TRANSITION,
			DEATH,
			RESET
		};
	};

	// Enum reference for gImages array of textures
	struct eImages
	{
		enum names
		{
			SPLASH,
			BACKGROUND,
			SHIP,
			SHIP_THRUST,
			ASTEROID1,
			ASTEROID2,
			BG_ASTEROID,
			SHEILD,
			SHEILD_BAR,
			SHEILD_BG_BAR,
			CIRCLE,
			B_BLACK_HOLE,
			SHEILD_PICKUP,
			TOTAL_IMAGES
		};
	};

	static SDL_Texture* game_images[eImages::TOTAL_IMAGES]; // array of pointers to Textures created (unreferenced by enImages enumerator)

	std::string image_path[eImages::TOTAL_IMAGES]
	{
		"images/Splash2.png",
		"images/background.png",
		"images/ship_normal.png",
		"images/ship_thrust.png",
		"images/ast1.png",
		"images/ast2.png",
		"images/backg_ast.png",
		"images/sheild.png",
		"images/sheild_bar.png",
		"images/sheild_background.png",
		"images/circle.png",
		"images/blueblackhole.png",
		"images/sheild_pickup.png"
	};

	// Enum reference for gImages array of textures
	struct eSounds
	{
		enum names
		{
			SHOOT,
			HIT_SML,
			HIT_MED,
			HIT_LRG,
			HIT_POWER,
			SHIP_HIT,
			SHIP_THRUST,
			COLLECT,
			BLACK_HOLE,
			ENTER_BH,
			DEATH,
			HIGH_SCORE,
			TOTAL_SOUNDS
		};
	};

	static Mix_Chunk* game_sounds[eSounds::TOTAL_SOUNDS];

	std::string sound_path[eSounds::TOTAL_SOUNDS]
	{
		"sounds/shoot.wav",
		"sounds/bang_small.wav",
		"sounds/bang_medium.wav",
		"sounds/bang_large.wav",
		"sounds/bang_power_up.wav",
		"sounds/ship_hit.wav",
		"sounds/ship_thrust.wav",
		"sounds/collect.wav",
		"sounds/blackhole.wav",
		"sounds/enter_bh.wav",
		"sounds/death.wav",
		"sounds/high_score.wav",
	};

private:
	void Init();
	void LoadMedia();
	void ExitGame();
	void CreateAsteroid(float x, float y, const float size, const bool isCollidable, const bool allowed_near_player, std::vector<std::unique_ptr<Entity>>& vector, const float screen_ratio);
	void Create2SubAsteroids(const Asteroid* const ast, std::vector<std::unique_ptr<Entity>>& vector);
	void CreateBackgroundAsteroids(const int amount);
	void HudDraw() const;

	SDL_Texture* LoadRenderedText(SDL_Texture* texture, const std::string textureText, const SDL_Color& const textColor, TTF_Font* const font, SDL_Rect& rect, const float s_r);
	SDL_Texture* LoadImageData(const std::string& path, bool& allMediaLoaded);

	DelayTimer shot_delay{ 200.0f , true };
	DelayTimer collision_delay{ 2500.0f , false };
	DelayTimer wave_delay{ 4000.0f , false };
	DelayTimer particle_delay{ 35.0f , true };

	int asteroid_amount = 2;
	int MAX_ASTEROID_SPEED = 250;
	int MIN_ASTEROID_SPEED = 75;

	int current_level = 1;
	int score = 0;

	const int ORIG_H = 930;
	const int ORIG_W = 1350;
	SDL_DisplayMode users_screen;

	int FONT_SIZE;
	float s_r;

	int SCREEN_HEIGHT;
	int SCREEN_WIDTH;
	int screen_center_x;
	int screen_center_y;

	int SHIP_THRUST;
	const int MAX_ANGLE_MOD = 36;
	int ANGLE_MODIFIER = MAX_ANGLE_MOD;
	int original_sheild_length;

	bool running = true;
	float smallest_asteroid;

	SDL_Color gtext_color = { 255, 255, 255 };	

	SDL_Texture* gScore = nullptr;
	SDL_Texture* sheild_amount = nullptr;
	SDL_Texture* sheild_bg_bar = nullptr;
	SDL_Texture* sheild_icon = nullptr;
	SDL_Rect* sheild_amount_rect = nullptr;
	SDL_Rect* sheild_bar_rect = nullptr;
	SDL_Rect* sheild_icon_rect = nullptr;

	SDL_Texture* lComplete = nullptr;

	SDL_Rect fullscreen{ 0,0, SCREEN_WIDTH, SCREEN_HEIGHT };
	SDL_Rect score_rect;

	std::vector<std::unique_ptr<Entity>> vec_bg_asteroids;
	std::vector<std::unique_ptr<Entity>> vec_asteroids;
	std::vector<std::unique_ptr<Entity>> vec_bullets;
	std::vector<std::unique_ptr<Particles>> vec_particles;
	std::vector<std::unique_ptr<Entity>> vec_power_ups;
	std::unique_ptr<Player> player;
};
