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

static const double PI = 3.14159265;

class Game {
public:
	Game(const int FPS);
	~Game();

	// Methods
	static SDL_Rect* GetRect(SDL_Texture* texture, int x, int y);
	static SDL_Event events;
	static SDL_Renderer* gRenderer;
	HighScore high_score{};
	//Globally used font
	TTF_Font* g_font = nullptr;
	TTF_Font* l_font = nullptr;

	void splash_screen(const int time_to_display);
	void handle_input(double& delta_time);
	void Update(double& delta_time);
	void render();

	bool is_running();
	void set_running(const bool new_bool);

	bool wave_complete = false;
	char play_again = NULL;
	int FPS;

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
			SHEILD_BAR,
			SHEILD_BG_BAR,
			CIRCLE,
			TOTAL_IMAGES
		};
	};

	static SDL_Texture* game_images[eImages::TOTAL_IMAGES]; // array of pointers to Textures created (unreferenced by enImages enumerator)

	std::string image_path[eImages::TOTAL_IMAGES]
	{
		"images/splash_image.png",
		"images/ship_normal.png",
		"images/ship_thrust.png",
		"images/ast1.png",
		"images/ast2.png",
		"images/backg_ast.png",
		"images/sheild_bar.png",
		"images/sheild_background.png",
		"images/circle.png",
	};

private:
	void init();
	void load_media();
	void exit_game();
	void handle_wave_completion();
	void CreateAsteroid(double x, double y, float size, bool isCollidable, bool allowed_near_player, std::vector<std::unique_ptr<Entity>>& vector, float screen_ratio);
	void Create2SubAsteroids(const Asteroid* const ast, std::vector<std::unique_ptr<Entity>>& vector);
	void CreateBackgroundAsteroids(int amount);
	void HudDraw();

	void Draw();

	//std::array<int, 2> level = { 1,100 };//{ 4, 5, 6, 7, 8 };// { 1, 1, 1, 1, 1}; //{ 4, 5, 6, 7, 8 };
	int asteroid_amount = 3;

	int current_level = 1;
	int score = 0;

	const int ORIG_H = 930;
	const int ORIG_W = 1350;
	SDL_DisplayMode users_screen;

	int SCREEN_HEIGHT;
	int SCREEN_WIDTH;
	int FONT_SIZE;
	float s_r;

	int screen_center_x;
	int screen_center_y;

	int SHIP_THRUST;
	const int ANGLE_MODIFIER = 35;
	int original_sheild_length;
	float current_sheild = 1.0f; // 1 = 100%

	DelayTimer shot_delay{ 200.0f , true};
	DelayTimer collision_delay{ 1500.0f , false};
	DelayTimer wave_delay{ 4000.0f , false};

	std::vector<std::unique_ptr<Entity>> vec_asteroids;
	std::vector<std::unique_ptr<Entity>> vec_bullets;

	bool running = true;
	float smallest_asteroid;

	SDL_Color gtext_color = { 255, 255, 255 };	
	
	void handle_death();
	SDL_Texture* LoadRenderedText(SDL_Texture* texture, std::string textureText, SDL_Color& textColor, TTF_Font* font, SDL_Rect& rect);
	SDL_Texture* load_image_data(std::string path, bool& allMediaLoaded);

	SDL_Texture* gScore = nullptr;
	SDL_Texture* sheild_amount = nullptr;
	SDL_Texture* sheild_bg_bar = nullptr;
	SDL_Rect* sheild_amount_rect;
	SDL_Rect* sheild_bar_rect;

	SDL_Texture* wComplete = nullptr;
	SDL_Texture* lComplete = nullptr;

	SDL_Texture* death1 = nullptr;
	SDL_Texture* death2 = nullptr;

	SDL_Rect death1_rect;
	SDL_Rect death2_rect;
	SDL_Rect score_rect;
	SDL_Rect complete;
	SDL_Window* gWindow = nullptr;
	std::unique_ptr<Player> player;
};
