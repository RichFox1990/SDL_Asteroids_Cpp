#include "Game.h"
#include "Asteroid.h"
#include "Player.h"
#include "Particles.h"
#include "Bullet.h"
#include "BlackHole.h"
#include <SDL_image.h>
#include <time.h>
#include "math.h"

#include "StateMachine/StateMachine.h"
#include "StateMachine/StateIntro.h"


SDL_Event Game::events;

SDL_Texture* Game::game_images[eImages::TOTAL_IMAGES];
Mix_Chunk* Game::game_sounds[eSounds::TOTAL_SOUNDS];

SDL_Renderer* Game::gRenderer = nullptr;


void Game::Init()
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
		// get users resloution
		SDL_GetDesktopDisplayMode(0, &users_screen);
		// use 70% of it
		SCREEN_HEIGHT = users_screen.h * .9;
		SCREEN_WIDTH = (float)SCREEN_HEIGHT * ((float)ORIG_W / (float)ORIG_H);

		// Find the ratio of this screen compared to the original fixed values to use as a scaling float number
		s_r = (float)SCREEN_WIDTH / (float)ORIG_W;

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
				else
				{
					//Initialize SDL_ttf
					if (TTF_Init() == -1)
					{
						printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
						success = false;
					}
					//Initialize SDL_mixer
					if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
					{
						printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
						success = false;
					}
					Mix_AllocateChannels(Game::eSounds::TOTAL_SOUNDS);
				}
			}
		}
	}
	set_running(success);
}


SDL_Texture* Game::LoadImageData(const std::string& path, bool& allMediaLoaded)
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


SDL_Rect* Game::GetRect(SDL_Texture* const texture, const int x, const int y)
{
	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	SDL_Rect* rect = new SDL_Rect{ x, y, w, h };
	return rect;
}


void Game::LoadMedia()
// Loads media for the game
{
	bool allMediaLoaded = true;

	for (int x = 0; x < eImages::TOTAL_IMAGES; x++) // For the amount of the value of the enum "enImages::TOTAL_IMAGES"
	{
		game_images[x] = Game::LoadImageData(image_path[x], allMediaLoaded);
	}

	//Open the font
	g_font = TTF_OpenFont("Fonts/Asteroid.ttf", FONT_SIZE);
	if (g_font == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		allMediaLoaded = false;
	}
	l_font = TTF_OpenFont("Fonts/Asteroid.ttf", FONT_SIZE*5);
	if (l_font == NULL)
	{
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		allMediaLoaded = false;
	}

	// Load Sounds
	for (int x = 0; x < eSounds::TOTAL_SOUNDS; x++) // For the amount of sounds
	{
		game_sounds[x] = Mix_LoadWAV(sound_path[x].c_str());
		if (game_sounds[x] == NULL)
		{
			printf("Failed to load %s sound effect! SDL_mixer Error: %s\n", sound_path[x].c_str(), Mix_GetError());
			allMediaLoaded = false;
		}
	}

	set_running(allMediaLoaded);
}


SDL_Texture* Game::LoadRenderedText(SDL_Texture* texture, const std::string textureText, const SDL_Color& const textColor, TTF_Font* const font, SDL_Rect& rect, const float s_r)
{
	if (texture != nullptr)
	{
		SDL_DestroyTexture(texture);
		texture = nullptr;
	}

	SDL_Texture* temp = nullptr;
	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, textureText.c_str(), textColor);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		temp = SDL_CreateTextureFromSurface(gRenderer, textSurface);
		if (temp == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}
		else
		{
			rect = SDL_Rect{ rect.x, rect.y, (int)((float)textSurface->w * s_r), (int)((float)textSurface->h * s_r) };
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	return temp;
}


void Game::CreateAsteroid(float x, float y, const float size, const bool isCollidable, const bool allowed_near_player, std::vector<std::unique_ptr<Entity>>& vector, const float s_r)
{
	int angle;
	float vx;
	float vy;

	// randomise starting angle of image
	angle = ((float)rand() / (float)RAND_MAX) * 360;

	// randomise radian angle of velocity
	vx = ((float)rand() / (float)RAND_MAX) * 6.283185f;
	vy = ((float)rand() / (float)RAND_MAX) * 6.283185f;

	float speed_multiplier = (((MAX_ASTEROID_SPEED) * ((float)rand() / (float)RAND_MAX))) + MIN_ASTEROID_SPEED;
	if (speed_multiplier > MAX_ASTEROID_SPEED)
	{
		speed_multiplier = MAX_ASTEROID_SPEED;
	}
	if (!isCollidable)
	{
		speed_multiplier /= 5;
	}
	vx = sin(vx) * speed_multiplier;
	vy = cos(vy) * speed_multiplier;

	// If these asteroids cant spawn near the player (new wave etc)
	if (!allowed_near_player)
	{
		// create temp entity
		Asteroid temp(x, y, vx, vy, angle, size, isCollidable, s_r);
		// enlarge the radius 
		temp.radius *= 6.0 * s_r;
		// use this entity to move the x,y coords if this asteroid is near the player using collision method
		while (player->Collision(temp))
		{
			vx < 0 ? temp.pos_x -= 10 : temp.pos_x += 10;
			vy < 0 ? temp.pos_y += 10 : temp.pos_y -= 10;
		}
		x = temp.pos_x;
		y = temp.pos_y;
	}
	// Push new asteroid into given vector
	vector.push_back(std::make_unique<Asteroid>(x, y, vx, vy, angle, size, isCollidable, s_r));
}


void Game::Create2SubAsteroids(const Asteroid* const ast, std::vector<std::unique_ptr<Entity>>& vector)
{
	// take radian angle of asteroid the 2 new ones will split from
	float radians = atan2(ast->vel_x, ast->vel_y);
	float radians2 = radians;

	// calculate an offset (max 1/4*PI (90 degrees)
	float offset = (PI / 4.0) * ((float)rand() / (float)RAND_MAX);
	// make sure they cant be too close to 0
	if (abs(offset) < .2) { offset = .2; }
	// set one to plus and other to minus of the offset (to imatate them splitting uniformly)
	radians += offset;
	radians2 -= offset;
	float previous_speed = sqrtf(powf(ast->vel_x, 2) + powf(ast->vel_y, 2));
	float limit = 1.0f;
	// if the original asteroid wasnt over half of the max speed. Limit the new speed to slightly over Max/2 (to stop asteroids going from slowest to max)
	if (previous_speed < MAX_ASTEROID_SPEED / 2.0f)
	{
		limit = 1.8f;
	}

	float speed_multiplier = ((((float)MAX_ASTEROID_SPEED / limit) * ((float)rand() / (float)RAND_MAX)) + ((float)MIN_ASTEROID_SPEED * (float)limit));
	float speed_multiplier2 = ((((float)MAX_ASTEROID_SPEED / limit) * ((float)rand() / (float)RAND_MAX)) + ((float)MIN_ASTEROID_SPEED * (float)limit));
	if (speed_multiplier > MAX_ASTEROID_SPEED)
	{
		speed_multiplier = MAX_ASTEROID_SPEED;
	}
	if (speed_multiplier2 > MAX_ASTEROID_SPEED)
	{
		speed_multiplier2 = MAX_ASTEROID_SPEED;
	}
	
	// Calculate the new velocity vector from these angles
	float vx = sin(radians) * speed_multiplier;
	float vy = cos(radians) * speed_multiplier;
	float vx2 = sin(radians2) * speed_multiplier2;
	float vy2 = cos(radians2) * speed_multiplier2;//distance) *100 / (rand() % 100 + 75);

	vec_particles.push_back(std::make_unique<Particles>(ast->pos_x + ast->center.x, ast->pos_y + ast->center.y, radians, speed_multiplier, offset, 10, s_r));
	vec_particles.push_back(std::make_unique<Particles>(ast->pos_x + ast->center.x, ast->pos_y + ast->center.y, radians2, speed_multiplier2, offset, 10, s_r));

	// randomise the image thats used (out of 2 possible images)
	float rand_img = ((float)rand() / (float)RAND_MAX);

	// push the asteroids into the given vector
	vector.push_back(std::make_unique<Asteroid>(ast->pos_x, ast->pos_y, vx, vy, ast->angle, ast->size - Asteroid::SMALL, ast->isCollidable, s_r));
	vector.push_back(std::make_unique<Asteroid>(ast->pos_x, ast->pos_y, vx2, vy2, ast->angle, ast->size - Asteroid::SMALL, ast->isCollidable, s_r));
}

void Game::CreateBackgroundAsteroids(const int amount)
{
	float x, y;
	for (int i = 0; i < amount; i++)
	{
		x = (((float)rand() / (float)RAND_MAX)) * (float)Game::SCREEN_WIDTH;;
		y = (((float)rand() / (float)RAND_MAX)) * (float)Game::SCREEN_HEIGHT;
		CreateAsteroid(x, y, Asteroid::SMALL, false, true, vec_bg_asteroids, s_r);
	}
}


void Game::Run()
{
	state_machine->Run();
}


void Game::HudDraw() const
{
	SDL_RenderCopy(Game::gRenderer, sheild_bg_bar, NULL, sheild_bar_rect);
	SDL_SetRenderDrawColor(Game::gRenderer, 204, 255, 255, 255);
	SDL_RenderFillRect(Game::gRenderer, sheild_amount_rect);
	SDL_RenderCopy(Game::gRenderer, sheild_icon, NULL, sheild_icon_rect);
	SDL_RenderCopy(Game::gRenderer, gScore, NULL, &score_rect);
}


Game::Game(const int FPS)
{
	this->FPS = FPS;

	state_machine = std::make_unique<StateMachine>(this);

	srand((unsigned)time(NULL));
	this->Init();
	FONT_SIZE = 50.0 * s_r;
	this->LoadMedia();

	SHIP_THRUST = 750 * s_r;
	MAX_ASTEROID_SPEED *= s_r;
	MIN_ASTEROID_SPEED *= s_r;
	smallest_asteroid = 2 * s_r;
	screen_center_x = SCREEN_WIDTH / 2;
	screen_center_y = SCREEN_HEIGHT / 2;

	vec_bg_asteroids.reserve(60);
	vec_asteroids.reserve(50);
	vec_bullets.reserve(20);
	vec_particles.reserve(25);
	vec_power_ups.reserve(5);

	// create background asteroids
	CreateBackgroundAsteroids(80);

	// create player
	player = std::make_unique<Player>(screen_center_x, screen_center_y, starting_sheild, s_r);

	// Create main asteroids
	float x, y;
	for (int i = 0; i < asteroid_amount + current_level; i++)
	{
		x = (((float)rand() / (float)RAND_MAX)) * (float)Game::SCREEN_WIDTH;;
		y = (((float)rand() / (float)RAND_MAX)) * (float)Game::SCREEN_HEIGHT;
		CreateAsteroid(x, y, Asteroid::LARGE, true, false, vec_asteroids, s_r);
	}

	//load text texture for score
	gScore = LoadRenderedText(gScore, "SCORE: " + std::to_string(score), gtext_color, g_font, score_rect, s_r);
	score_rect.x = SCREEN_WIDTH/2 - score_rect.w / 2;
	score_rect.y += 10 * s_r;

	//set sheild icon image
	sheild_icon = Game::game_images[Game::eImages::SHEILD];
	//set image for sheild HUD
	sheild_bg_bar = Game::game_images[Game::eImages::SHEILD_BG_BAR];
	// get the rects
	sheild_icon_rect = Game::GetRect(sheild_icon, 25.0f * s_r, 5.0f * s_r);
	sheild_bar_rect = GetRect(sheild_amount, 45.0f * s_r, 18.0f * s_r);
	sheild_amount_rect = GetRect(sheild_amount, 55.0f * s_r, 28.0f * s_r);
	// scale using screen height and width
	sheild_bar_rect->h = SCREEN_HEIGHT / 25;
	sheild_bar_rect->w = SCREEN_WIDTH / 2.5;
	sheild_icon_rect->h = sheild_bar_rect->h * 1.5;
	sheild_icon_rect->w = sheild_icon_rect->h * (150.0f/180.0f);
	sheild_amount_rect->h = sheild_bar_rect->h - 20.0f * s_r;
	sheild_amount_rect->w = sheild_bar_rect->w - 20.0f * s_r;
	// save original size
	original_sheild_length = sheild_amount_rect->w;
	// scale the shown sheild amount by current sheild amount (1 = 100%, 0.5 = 50% etc)
	sheild_amount_rect->w = original_sheild_length * player->current_sheild;

	state_machine->ChangeState(new StateIntro);
}


bool Game::is_running() const
// Gets the game's current "running" loop condition
{
	return running;
}


void Game::set_running(const bool new_bool)
// Sets the game main "running" loop condition
{
	running = new_bool;
}


void Game::ExitGame()
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
	}
	
	TOTAL = eSounds::TOTAL_SOUNDS;
	for (int x = 0; x < TOTAL; x++)
	{
		Mix_FreeChunk(game_sounds[x]);
		game_sounds[x] = nullptr;
	}

	SDL_DestroyTexture(gScore);
	gScore = nullptr;

	if (lComplete != nullptr)
	{
		SDL_DestroyTexture(lComplete);
		lComplete = nullptr;
	}

	delete sheild_amount_rect;
	sheild_amount_rect = nullptr;
	delete sheild_bar_rect;
	sheild_bar_rect = nullptr;

	// Destroy Window and set pointer to NULL
	SDL_DestroyWindow(gWindow);
	gWindow = nullptr;
	SDL_DestroyRenderer(gRenderer);
	gRenderer = nullptr;

	//Free global font
	TTF_CloseFont(g_font);
	TTF_CloseFont(l_font);
	g_font = nullptr;
	l_font = nullptr;

	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();
}


Game::~Game()
{
	ExitGame();
}