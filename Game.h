#pragma once
#include <iostream>
#include <string>
#include "SDL.h"
#include <vector>
#include <array>
#include "Player.h"
#include "DelayTimer.h"
#include "HighScore.h"
#include <SDL_ttf.h>
#include "Asteroid.h"
#include "Particles.h"
#include <SDL_mixer.h>

static const double PI = 3.14159265;

class Game {
public:
	Game(const int FPS);
	~Game();

	// Methods
	static SDL_Rect* GetRect(SDL_Texture* const texture, const int x, const int y);
	static SDL_Event events;
	static SDL_Renderer* gRenderer;

	HighScore high_score{};
	//Globally used font
	TTF_Font* g_font = nullptr;
	TTF_Font* l_font = nullptr;

	void SplashScreen(const int time_to_display) const;
	void HandleInput(const double delta_time);
	void Update(const double delta_time);
	void Render() const;

	bool is_running() const;
	void set_running(const bool new_bool);

	bool wave_complete = false;
	mutable bool game_reset = false;
	char play_again = NULL;
	int FPS;

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
			OUTER_PICKUP,
			TOTAL_IMAGES
		};
	};

	static SDL_Texture* game_images[eImages::TOTAL_IMAGES]; // array of pointers to Textures created (unreferenced by enImages enumerator)

	std::string image_path[eImages::TOTAL_IMAGES]
	{
		"images/Splash2.png",
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
		"images/outer_pickup.png"
	};

	// Enum reference for gImages array of textures
	struct eSounds
	{
		enum names
		{
			SHOOT,
			HIT,
			SHIP_HIT,
			SHIP_THRUST,
			SHIELD_UP,
			BLACK_HOLE,
			ENTER_BH,
			HIGH_SCORE,
			TOTAL_SOUNDS
		};
	};

	static Mix_Chunk* game_sounds[eSounds::TOTAL_SOUNDS];

	std::string sound_path[eSounds::TOTAL_SOUNDS]
	{
		"sounds/shoot.wav",
		"sounds/hit.wav",
		"sounds/ship_hit.wav",
		"sounds/ship_thrust.wav",
		"sounds/shield_up.wav",
		"sounds/blackhole.wav",
		"sounds/enter_bh.wav",
		"sounds/high_score.wav",
	};

private:
	void Init();
	void LoadMedia();
	void ResetGame();
	void ExitGame();
	void HandleWaveCompletion();
	void CreateAsteroid(double x, double y, const float size, const bool isCollidable, const bool allowed_near_player, std::vector<std::unique_ptr<Entity>>& vector, const float screen_ratio);
	void Create2SubAsteroids(const Asteroid* const ast, std::vector<std::unique_ptr<Entity>>& vector);
	void CreateBackgroundAsteroids(const int amount);
	void NextLevelTransition(const int center_x, const int center_y);
	void HandleDeath();
	void HudDraw() const;
	void Draw() const;
	SDL_Texture* LoadRenderedText(SDL_Texture* texture, const std::string textureText, const SDL_Color& const textColor, TTF_Font* const font, SDL_Rect& const rect, const float s_r);
	SDL_Texture* LoadImageData(const std::string& path, bool& allMediaLoaded);

	DelayTimer shot_delay{ 200.0f , true };
	DelayTimer collision_delay{ 1500.0f , false };
	DelayTimer wave_delay{ 4000.0f , false };
	DelayTimer particle_delay{ 35.0f , true };

	//std::array<int, 2> level = { 1,100 };//{ 4, 5, 6, 7, 8 };// { 1, 1, 1, 1, 1}; //{ 4, 5, 6, 7, 8 };
	int asteroid_amount = 3;
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

	SDL_Texture* wComplete = nullptr;
	SDL_Texture* lComplete = nullptr;

	SDL_Texture* death1 = nullptr;
	SDL_Texture* death2 = nullptr;

	SDL_Rect death1_rect;
	SDL_Rect death2_rect;
	SDL_Rect score_rect;
	SDL_Rect complete;
	SDL_Window* gWindow = nullptr;

	std::vector<std::unique_ptr<Entity>> vec_bg_asteroids;
	std::vector<std::unique_ptr<Entity>> vec_asteroids;
	std::vector<std::unique_ptr<Entity>> vec_bullets;
	std::vector<std::unique_ptr<Particles>> vec_particles;
	std::unique_ptr<Player> player;
};
