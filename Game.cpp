#include "Game.h"
#include <iostream>
#include "Bullet.h"
//#include "..\Maths\MyMaths.h"


using namespace std;

SDL_Event Game::events;

ImageData* Game::game_images[eImages::TOTAL_IMAGES];

SDL_Renderer* Game::gRenderer = nullptr;


void Game::init()
/* Initializes the SDL subsystems and creates game window and game surface
return "bool" to indicate successful operation or not*/
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO + SDL_INIT_AUDIO) < 0) {

		cout << "Error initializing SDL_INIT : " << SDL_GetError() << endl;

		success = false;
	}

	else
	{
		//Create window
		gWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			cout << "Window could not be created! SDL Error: " << SDL_GetError() << endl;
			success = false;
		}
		else
		{
			//initialize Renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

			if (gRenderer == NULL)
			{
				cout << "Renderer failed to init. ERROR: " << SDL_GetError() << endl;
				success = false;
			}

			else
			{
				//Initialize renderer color
				SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255); // Black

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags))
				{
					cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError();
					success = false;
				}
			}
		}
	}
	set_running(success);
}


void Game::load_media()
// Loads media for the game
{
	bool allMediaLoaded = true;

	for (int x = 0; x < eImages::TOTAL_IMAGES; x++) // For the amount of the value of the enum "enImages::TOTAL_IMAGES"
	{
		game_images[x] = TextureManager::load_image_data(image_path[x], allMediaLoaded);
	}

	set_running(allMediaLoaded);
}



bool Game::is_running()
// Gets the game's current "running" loop condition
{
	return running;
}


void Game::set_running(const bool new_bool)
// Sets the game main "running" loop condition
{
	running = new_bool;
}


void Game::splash_screen(const int time_to_display)
// Displays and delays the splash screen logo
{

	SDL_RenderCopy(gRenderer, game_images[eImages::SPLASH]->texture, NULL, NULL);

	//Update screen
	SDL_RenderPresent(gRenderer);
	SDL_Delay(time_to_display * 1000);
}


void Game::Draw()
// Draws the various shapes onto the screen
{
	if (entities.size() != 0)
	{
		for (auto& ent : entities)
		{
			ent->Draw();
		}
	}
}


void Game::handle_input(const double& dt)
{
	while (SDL_PollEvent(&events) != 0)
	{
		if (events.type == SDL_QUIT)
		{
			cout << "exit" << endl;
			set_running(false);
		}

		else if (events.type == SDL_KEYDOWN)
		{
			switch (events.key.keysym.sym)
			{
				cout << "A pressed";
			case SDLK_SPACE:
				entities.push_back(std::make_unique<Bullet>(player));
				break;
			}
		}
	
	}
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	//Get the keystates
	//Uint8* keystates = SDL_GetKeyState(NULL);
	if (keystates[SDL_SCANCODE_W])
	{
		player->vel.x += sin(player->angle * PI / 180) * 600.0f * dt;
		player->vel.y += -cos(player->angle * PI / 180) * 600.0f * dt;
	}
	if (keystates[SDL_SCANCODE_A])
	{
		player->angle -= 500.0 * dt;
		player->angle = player->angle % 360;
	}
	if (keystates[SDL_SCANCODE_D])
	{
		player->angle += 500 * dt;
		player->angle = player->angle % 360;
	}
}


void Game::render()
{
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);
	Draw();
	//SDL_RenderCopy(gRenderer, menuImages[enImages::BACKGROUND], NULL, NULL);
	//viewPortRender();

	// Reset viewport to full screen of window
	//SDL_RenderSetViewport(gRenderer, NULL);

	// render an image at given Rect
	//SDL_RenderCopy(gRenderer, gImages[eImages::LEFT], gImages[eImages::LEFT], &topRightViewport);

	SDL_RenderPresent(gRenderer);
}


void Game::Update(const double& dt)
// Main object update loop for the game
{
	for (auto& ent : entities)
	{
		ent->Update(dt);
		ent->WrapCoords();
	}
	/*int i = 0;
	for (auto& ent : entities)
	{
		if (ent->is_dead)
		{
			entities.erase(entities.begin() + i);
		}
		i++;
	}*/
	/*if (entities.size() > 0)
	{
		auto i = remove_if(entities.begin(), entities.end(), [&](std::unique_ptr<Entity> e) { return (e->is_dead); });
		if (i != entities.end())
			entities.erase(i);
	}*/
	//std::cout << player->angle << std::endl;
}


void Game::exit_game()
/* Free surfaces and set any pointers to Null before exiting
IMPORTANT FOR MEMORY USAGE INSIDE PROGRAM */
{
	cout << "Cleaning up code";
	SDL_Delay(250);

	int TOTAL = eImages::TOTAL_IMAGES;
	for (int x = 0; x < TOTAL; x++)
	{
		SDL_DestroyTexture(game_images[x]->texture);
		game_images[x]->texture = nullptr;

		/*delete game_images[x]->rect;
		game_images[x]->rect = nullptr;

		delete gImages[x];
		gImages[x] = nullptr;*/
	}
	// Destroy Window and set pointer to NULL
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;

	IMG_Quit();
	SDL_Quit();
}

Game::Game()
{
	this->init();
	this->load_media();
	entities.push_back(make_unique<Player>(screen_center.x, screen_center.y));
	player = entities[0].get();

}

Game::~Game()
{
	exit_game();
}