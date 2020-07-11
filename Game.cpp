#include "Game.h"
#include "Asteroid.h"
#include "Bullet.h"
#include <SDL_image.h>

SDL_Event Game::events;

SDL_Texture* Game::game_images[eImages::TOTAL_IMAGES];

SDL_Renderer* Game::gRenderer = nullptr;


void Game::init()
/* Initializes the SDL subsystems and creates game window and game surface
return "bool" to indicate successful operation or not*/
{
	bool success = true;

	if (SDL_Init(SDL_INIT_VIDEO + SDL_INIT_AUDIO) < 0) {

		std::cout << "Error initializing SDL_INIT : " << SDL_GetError() << std::endl;

		success = false;
	}

	else
	{
		//Create window
		gWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (gWindow == NULL)
		{
			std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			//initialize Renderer
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);

			if (gRenderer == NULL)
			{
				std::cout << "Renderer failed to init. ERROR: " << SDL_GetError() << std::endl;
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
					std::cout << "SDL_image could not initialize! SDL_image Error: " << IMG_GetError();
					success = false;
				}
			}
		}
	}
	set_running(success);
}

SDL_Texture* Game::load_image_data(std::string path, bool& allMediaLoaded)
// ( Called in the loadMedia() method )
// This loads the texture of the given path, then converts to the texture and returns the pointer to it.
{
	SDL_Texture* newTexture = nullptr;
	std::cout << path << std::endl;

	// load initial surface image
	SDL_Surface* tempSurface = IMG_Load(path.c_str());
	if (tempSurface == NULL)
	{
		std::cout << path << " failed to load: SDL ERROR: " << IMG_GetError();
		allMediaLoaded = false;
	}
	else
	{
		std::cout << "converting";
		//Convert surface to a texture
		newTexture = SDL_CreateTextureFromSurface(Game::gRenderer, tempSurface);
		if (newTexture == NULL)
		{
			std::cout << "Unable to create texture " << path << " SDL Error: " << SDL_GetError() << std::endl;
			allMediaLoaded = false;
		}
		//Get rid of old temp loaded surface - VITAL!

		SDL_FreeSurface(tempSurface);

	}

	return newTexture;
}

SDL_Rect* Game::GetRect(SDL_Texture* texture, int x, int y)
{
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect* rect = new SDL_Rect{ x, y, w, h };
	return rect;
}

void Game::load_media()
// Loads media for the game
{
	bool allMediaLoaded = true;

	for (int x = 0; x < eImages::TOTAL_IMAGES; x++) // For the amount of the value of the enum "enImages::TOTAL_IMAGES"
	{
		game_images[x] = Game::load_image_data(image_path[x], allMediaLoaded);
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

	SDL_RenderCopy(gRenderer, game_images[eImages::SPLASH], NULL, NULL);

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
			std::cout << "exit" << std::endl;
			set_running(false);
		}

		else if (events.type == SDL_KEYDOWN)
		{
			switch (events.key.keysym.sym)
			{
				std::cout << "A pressed";
			case SDLK_SPACE:
				//entities.push_back(std::make_unique<Bullet>(player));
				break;
			}
		}
	
	}
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	//Get the keystates
	//Uint8* keystates = SDL_GetKeyState(NULL);
	if (keystates[SDL_SCANCODE_W])
	{
		player->vel_x += sin(player->angle * PI / 180) * SHIP_THRUST * dt;
		player->vel_y += -cos(player->angle * PI / 180) * SHIP_THRUST * dt;
	}
	if (keystates[SDL_SCANCODE_A])
	{
		player->angle -= ANGLE_MODIFIER * dt;
		player->angle = player->angle % 360;
	}
	if (keystates[SDL_SCANCODE_D])
	{
		player->angle += ANGLE_MODIFIER * dt;
		player->angle = player->angle % 360;
	}
	if (keystates[SDL_SCANCODE_SPACE])
	{
		if (shot_delay.Check())
		{
			entities.push_back(std::make_unique<Bullet>(player));
		}
	}
}


void Game::render()
{
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);
	Draw();

	SDL_RenderPresent(gRenderer);
}

void Game::CreateAsteroids(int amount, int size, bool isCollidable, bool allowed_near_player)
{
	int angle;
	double x;
	double y;
	double vx;
	double vy;
	double rand_img;

	for (int i = 0; i < amount; i++)
	{
		angle = ((double)rand() / (double)RAND_MAX) * 360;
		x = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_WIDTH;;
		y = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_HEIGHT;
		vx = ((double)rand() / (double)RAND_MAX) * 6.283185f;
		vy = ((double)rand() / (double)RAND_MAX) * 6.283185f;
		vx = sin(vx) * 125.0;
		vy = cos(vx) * 125.0;

		rand_img = ((float)rand() / (float)RAND_MAX);

		entities.push_back(std::make_unique<Asteroid>(x, y, vx, vy, angle, size, rand_img, isCollidable));
	}
}


void Game::Update(const double& dt)
// Main object update loop for the game
{
	for (auto& ent : entities)
	{
		ent->Update(dt);
		ent->WrapCoords();
	}

	// I entity is marked as dead, remove them
	entities.erase(std::remove_if(entities.begin(), entities.end(), [](std::unique_ptr<Entity>& e) { return (e->is_dead); }), entities.end());
}


void Game::exit_game()
/* Free surfaces and set any pointers to Null before exiting
IMPORTANT FOR MEMORY USAGE INSIDE PROGRAM */
{
	std::cout << "Cleaning up code";
	SDL_Delay(250);

	int TOTAL = eImages::TOTAL_IMAGES;
	for (int x = 0; x < TOTAL; x++)
	{
		SDL_DestroyTexture(game_images[x]);
		game_images[x] = nullptr;

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

	entities.reserve(100);

	// create background asteriods
	CreateAsteroids(15, Asteroid::MEDIUM, false, true);
	CreateAsteroids(15, Asteroid::LARGE, false, true);
	CreateAsteroids(15, Asteroid::SMALL, false, true);

	CreateAsteroids(5, Asteroid::LARGE, true, true);

	entities.push_back(std::make_unique<Player>(screen_center_x, screen_center_y));
	player = entities.back().get();
}

Game::~Game()
{
	exit_game();
}