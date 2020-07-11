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
	void init();
	SDL_Texture* load_image_data(std::string path, bool& allMediaLoaded);
	static SDL_Rect* GetRect(SDL_Texture* texture, int x, int y);
	void load_media();
	void splash_screen(const int time_to_display);
	void exit_game();
	void set_running(const bool new_bool);
	bool is_running();

	void CreateAsteroids(int amount, int size, bool isCollidable, bool allowed_near_player);

	void Draw();

	void Update(const double& delta_time);
	void handle_input(const double& delta_time);
	void render();

	// Variables

	static const int SCREEN_HEIGHT = 620 * 1.5;
	static const int SCREEN_WIDTH = 900 * 1.5;
	int screen_center_x = SCREEN_WIDTH / 2;
	int screen_center_y = SCREEN_HEIGHT / 2;

	const int SHIP_THRUST = 750;
	const int ANGLE_MODIFIER = 350;

	DelayTimer shot_delay{ 250.0f };

	// Enum reference for gImages array of textures
	struct eImages
	{
		enum names
		{
			SPLASH,
			SHIP,
			ASTEROID1,
			ASTEROID2,
			TOTAL_IMAGES
		};
	};

	std::string image_path[eImages::TOTAL_IMAGES]
	{
		"images/splash_image.png",
		"images/ship.png",
		"images/astroid1.png",
		"images/astroid2.png",
	};

	static SDL_Event events;

	static SDL_Renderer* gRenderer;

	static SDL_Texture* game_images[eImages::TOTAL_IMAGES]; // array of pointers to Textures created (unreferenced by enImages enumerator)


private:
	bool running = true;
	SDL_Window* gWindow = nullptr;
	Entity* player;
	std::vector<std::unique_ptr<Entity>> entities;
};
