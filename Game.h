#pragma once
#include <iostream>
#include "SDL.h"
#include <vector>
#include "Player.h"
#include "DelayTimer.h"

static const double PI = 3.14159265;

class Game {
public:
	Game();
	~Game();

	// Methods
	static SDL_Rect* GetRect(SDL_Texture* texture, int x, int y);
	static SDL_Event events;
	static SDL_Renderer* gRenderer;

	void splash_screen(const int time_to_display);
	void Update(const double& delta_time);
	void handle_input(const double& delta_time);
	void render();

	bool is_running();
	void set_running(const bool new_bool);

	// Enum reference for gImages array of textures
	struct eImages
	{
		enum names
		{
			SPLASH,
			SHIP,
			ASTEROID1,
			ASTEROID2,
			CIRCLE,
			TOTAL_IMAGES
		};
	};

	static SDL_Texture* game_images[eImages::TOTAL_IMAGES]; // array of pointers to Textures created (unreferenced by enImages enumerator)

	std::string image_path[eImages::TOTAL_IMAGES]
	{
		"images/splash_image.png",
		"images/ship.png",
		"images/astroid1.png",
		"images/astroid2.png",
		"images/circle.png",
	};

private:
	void init();
	void load_media();
	void exit_game();
	void CreateAsteroid(double x, double y, float size, bool isCollidable, bool allowed_near_player, std::vector<std::unique_ptr<Entity>>& vector, float screen_ratio);
	void Draw();

	const int ORIG_H = 930;
	const int ORIG_W = 1350;
	SDL_DisplayMode users_screen;

	int SCREEN_HEIGHT;
	int SCREEN_WIDTH;
	float s_r;

	int screen_center_x;
	int screen_center_y;

	int SHIP_THRUST;
	const int ANGLE_MODIFIER = 350;

	DelayTimer shot_delay{ 200.0f };
	DelayTimer collision_delay{ 3000.0f };

	std::vector<std::unique_ptr<Entity>> vec_asteroids;
	std::vector<std::unique_ptr<Entity>> vec_bullets;

	bool running = true;
	float smallest_asteroid;

	SDL_Texture* load_image_data(std::string path, bool& allMediaLoaded);
	SDL_Window* gWindow = nullptr;
	std::unique_ptr<Entity> player;
};
