#pragma once
#include <iostream>
#include "TextureManager.h"
#include "ImageData.h"
#include "SDL.h"
#include "SDL_Timer.h"
#include <vector>
#include "Point.h"
#include "Entity.h"
#include "Player.h"
//#include "..\Maths\MyMaths.h"


class Game {
public:
	Game();
	~Game();
	// Methods
	void init();
	void load_media();
	void splash_screen(const int time_to_display);
	void exit_game();
	void set_running(const bool new_bool);
	bool is_running();

	void Draw();
	void viewPortRender();

	void Update(const double& delta_time);
	void handle_input(const double& delta_time);
	void render();

	// Variables

	static const int SCREEN_HEIGHT = 620*1.5;
	static const int SCREEN_WIDTH = 900*1.5;
	const double PI = 3.14159265;

	SDL_Rect full_viewport{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
	Point screen_center{ SCREEN_WIDTH/2, SCREEN_HEIGHT/2 };

	// Enum reference for menuImagesName and the gImages array
	struct eImages
	{
		enum names
		{
			SPLASH,
			SHIP,
			ASTROID1,
			ASTRIOD2,
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

	static ImageData* game_images[eImages::TOTAL_IMAGES]; // array of pointers to image info created in the ImageData struct (unreferenced by enImages enumerator)


private:
	bool running = true;
	SDL_Window* gWindow = nullptr;
	Entity* player;
	std::vector<std::unique_ptr<Entity>> entities;
};
