#include "Game.h"
#include "Asteroid.h"
#include "Bullet.h"
#include <SDL_image.h>
#include <time.h>


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
		// get users resloution
		SDL_GetDesktopDisplayMode(0, &users_screen);
		// use 70% of it
		SCREEN_HEIGHT = users_screen.h * .9;
		SCREEN_WIDTH = (float)SCREEN_HEIGHT * ((float)ORIG_W / (float)ORIG_H);
		// Find the ratio of this screen compared to the original fixed values to use as a scaling float number
		s_r = (float)SCREEN_WIDTH / ORIG_W;

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


SDL_Texture* Game::LoadRenderedText(std::string textureText, SDL_Color textColor, TTF_Font* font, SDL_Rect& rect)
{
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
			if (rect.w == 0)
			{
				rect = SDL_Rect{ 0, 0, textSurface->w, textSurface->h };
			}
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}

	return temp;
}


void Game::load_media()
// Loads media for the game
{
	bool allMediaLoaded = true;

	for (int x = 0; x < eImages::TOTAL_IMAGES; x++) // For the amount of the value of the enum "enImages::TOTAL_IMAGES"
	{
		game_images[x] = Game::load_image_data(image_path[x], allMediaLoaded);
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


void Game::handle_input(double& dt)
{
	dt > 2 ? dt = 0 : dt = dt;
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
			case SDLK_SPACE:
				//entities.push_back(std::make_unique<Bullet>(player));
				break;			
			case SDLK_w:
				player->img = Game::game_images[Game::eImages::SHIP_THRUST];
				break;
			case SDLK_i:
				player->debug = !(player->debug);
				break;			
			case SDLK_UP:
				player->img = Game::game_images[Game::eImages::SHIP_THRUST];
				break;

			}
		}
		else if (events.type == SDL_KEYUP)
		{
			switch (events.key.keysym.sym)
			{
			case SDLK_w:
				player->img = Game::game_images[Game::eImages::SHIP];
				break;
			case SDLK_UP:
				player->img = Game::game_images[Game::eImages::SHIP];
				break;
			}
			}
	
	}
	const Uint8* keystates = SDL_GetKeyboardState(NULL);
	//Get the keystates
	//Uint8* keystates = SDL_GetKeyState(NULL);
	if (keystates[SDL_SCANCODE_W] || keystates[SDL_SCANCODE_UP])
	{
		player->vel_x += sin(player->angle * PI / 180) * SHIP_THRUST * dt;
		player->vel_y += -cos(player->angle * PI / 180) * SHIP_THRUST * dt;
	}
	if (keystates[SDL_SCANCODE_A] || keystates[SDL_SCANCODE_LEFT])
	{
		player->angle -= ANGLE_MODIFIER * dt;
		player->angle = player->angle % 360;
	}
	if (keystates[SDL_SCANCODE_D] || keystates[SDL_SCANCODE_RIGHT])
	{
		player->angle += ANGLE_MODIFIER * dt;
		player->angle = player->angle % 360;
	}
	if (keystates[SDL_SCANCODE_SPACE] && (vec_asteroids.size() > 45))
	{
		if (shot_delay.Check())
		{
			vec_bullets.push_back(std::make_unique<Bullet>(player.get(), s_r));
		}
	}
}


void Game::render()
{
	SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
	SDL_RenderClear(gRenderer);
	Draw();
	SDL_RenderCopy(Game::gRenderer, gScore, NULL, &score_rect);
	SDL_RenderPresent(gRenderer);
}

void Game::CreateAsteroid(double x, double y, float size, bool isCollidable, bool allowed_near_player, std::vector<std::unique_ptr<Entity>>& vector, float s_r)
{
	int angle;
	double vx;
	double vy;
	double rand_img;

	angle = ((double)rand() / (double)RAND_MAX) * 360;
	vx = ((double)rand() / (double)RAND_MAX) * 6.283185f;
	vy = ((double)rand() / (double)RAND_MAX) * 6.283185f;
	vx = sin(vx) * 125.0;
	vy = cos(vx) * 125.0;

	rand_img = ((float)rand() / (float)RAND_MAX);

	if (!allowed_near_player)
	{
		// create temp entity
		Asteroid temp(x, y, vx, vy, angle, size, rand_img, isCollidable, s_r);
		// enlarge the radius 
		temp.radius *= 6.0 * s_r;
		// use this entity to move the x,y coords if this asteroid is near the player
		while (player->Collision(temp))
		{
			vx < 0 ? temp.pos_x -= 10 : temp.pos_x += 10;
			vy < 0 ? temp.pos_y += 10 : temp.pos_y -= 10;
			//std::cout << "Spawn Collision detected!!" << std::endl;
		}
		x = temp.pos_x;
		y = temp.pos_y;
	}

	vector.push_back(std::make_unique<Asteroid>(x, y, vx, vy, angle, size, rand_img, isCollidable, s_r));

}


void Game::Update(double& dt)
// Main object update loop for the game
{
	dt > 2 ? dt = 0 : dt = dt;
	player->Update(dt);
	player->WrapCoords(SCREEN_WIDTH, SCREEN_HEIGHT);

	if (vec_asteroids.size() != 0)
	{
		for (auto& ast : vec_asteroids)
		{
			if (ast->isCollidable)
			{
				if ((player->Collision(*ast)) && (collision_delay.Check()))
				{
					//std::cout << "COLLISION PLAYER" << std::endl;
					if (score > high_score.top3[2].second)
					{
						std::string name;
						std::cout << "\nYou scored in the top 3. Please type your name(no spaces): " << std::endl;
						std::cin >> name;
						std::cout << std::endl;
						high_score.PushScore(name, score);
						high_score.Print();
					}
					else
					{
						std::cout << "\nYou didn't score in the top 3. Better luck next time\n" << std::endl;
					}
					set_running(false);
				}
			}
			ast->Update(dt);
			ast->WrapCoords(SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}


	if (vec_bullets.size() != 0)
	{
		// create temp vector to store any newly spawned asteroids (so we dont crash the vector we are iterating over)
		std::vector<std::unique_ptr<Entity>> vec_tempAsteroids;

		//loop through bullets
		for (auto& bul : vec_bullets)
		{
			// for each bullet check collision with all collidable asteroids
			if (vec_asteroids.size() != 0)
			{
				for (auto& ast : vec_asteroids)
				{
					if (ast->isCollidable)
					{
						// if collision
						if (bul->Collision(*ast)) 
						{
							bul->is_dead = true;
							ast->is_dead = true;
							score += 100;
							gScore = LoadRenderedText("SCORE: " + std::to_string(score), gtext_color, g_font, score_rect);

							if (ast->size > Asteroid::SMALL)
							{
								for (int i = 0; i < 2; i++)
									CreateAsteroid(ast->pos_x, ast->pos_y, ast->size - 2.0f, true, true, vec_tempAsteroids, s_r);
							}
						}
					}
				}
				bul->Update(dt);
				bul->WrapCoords(SCREEN_WIDTH, SCREEN_HEIGHT);
			}
		}
		// merge the new spawned asteroids into the main vector
		if (vec_tempAsteroids.size() != 0)
		{
			std::move(std::begin(vec_tempAsteroids), std::end(vec_tempAsteroids), std::back_inserter(vec_asteroids));
		}
	}

	// If entity is marked as dead, remove them
	vec_bullets.erase(std::remove_if(vec_bullets.begin(), vec_bullets.end(), [](std::unique_ptr<Entity>& e) { return (e->is_dead); }), vec_bullets.end());
	vec_asteroids.erase(std::remove_if(vec_asteroids.begin(), vec_asteroids.end(), [](std::unique_ptr<Entity>& e) { return (e->is_dead); }), vec_asteroids.end());

	// if only background asteroids
	if ((vec_asteroids.size() == 45) && (vec_bullets.size() == 0))
	{
		//if all levels complete
		if (level.size() - 1 == current_level)
		{
			// load game complete texture
			if (lComplete == nullptr)
			{
				complete.x = SCREEN_WIDTH / 2 - complete.w / 2;
				complete.y = SCREEN_HEIGHT / 2 - complete.h / 2;
				wave_delay.last_success = wave_delay.current_check = SDL_GetTicks();
				lComplete = LoadRenderedText("GAME COMPLETE - WIP", gtext_color, l_font, complete);
			}
			std::cout << "YOU WON" << std::endl;
			// Push scores is in top 3
			if (score > high_score.top3[2].second)
			{
				std::string name;
				std::cout << "\nYou scored in the top 3. Please type your name(no spaces): " << std::endl;
				SDL_RenderCopy(gRenderer, lComplete, NULL, &complete);
				SDL_RenderPresent(gRenderer);
				std::cin >> name;
				high_score.PushScore(name, score);
				high_score.Print();
				set_running(false);
			}
			else
			{
				std::cout << "\nYou didn't score in the top 3. Better luck next time\n" << std::endl;
				high_score.Print();
				SDL_RenderCopy(gRenderer, lComplete, NULL, &complete);
				SDL_RenderPresent(gRenderer);
				SDL_Delay(3000.0);
				set_running(false);
			}
			SDL_DestroyTexture(lComplete);
			
		}
		// if more levels to go
		else if (level.size() - 1 > current_level)
		{
			wComplete = LoadRenderedText("WAVE " + std::to_string(current_level + 1) + " COMPLETE", gtext_color, l_font, complete);
			complete.x = SCREEN_WIDTH/2 - complete.w / 2;
			complete.y = SCREEN_HEIGHT/2 - complete.h / 2;
			double x, y;
			current_level++;
			for (int i = 0; i < level[current_level]; i++)
			{
				x = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_WIDTH;
				y = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_HEIGHT;
				CreateAsteroid(x, y, Asteroid::LARGE, true, false, vec_asteroids, s_r);
			}

			wave_delay.last_success = wave_delay.current_check = SDL_GetTicks();
			while (!wave_delay.Check())
			{
				SDL_RenderCopy(gRenderer, wComplete, NULL, &complete);
				SDL_RenderPresent(gRenderer);
			}
			SDL_DestroyTexture(wComplete);
		}
	}
}


void Game::Draw()
// Draws the various shapes onto the screen
{
	if (vec_asteroids.size() != 0)
	{
		for (auto& ent : vec_asteroids)
		{
			ent->Draw();
		}
	}

	if (vec_bullets.size() != 0)
	{
		for (auto& ent : vec_bullets)
		{
			ent->Draw();
		}
	}

	player->Draw();
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
	}
	SDL_DestroyTexture(gScore);
	gScore = nullptr;

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
	SDL_Quit();
}

Game::Game()
{
	srand((unsigned)time(NULL));
	this->init();
	FONT_SIZE = 50.0 * s_r;
	this->load_media();

	SHIP_THRUST = 750 * s_r;
	smallest_asteroid = 2 * s_r;
	screen_center_x = SCREEN_WIDTH / 2;
	screen_center_y = SCREEN_HEIGHT / 2;

	vec_asteroids.reserve(60);
	vec_bullets.reserve(20);

	// create background asteriods
	double x, y;
	for (int i = 0; i < 15; i++)
	{
		x = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_WIDTH;;
		y = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_HEIGHT;
		CreateAsteroid(x, y, Asteroid::SMALL-1, false, true, vec_asteroids, s_r);

		x = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_WIDTH;;
		y = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_HEIGHT;
		CreateAsteroid(x, y, Asteroid::MEDIUM-1, false, true, vec_asteroids, s_r);

		x = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_WIDTH;;
		y = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_HEIGHT;
		CreateAsteroid(x, y, Asteroid::LARGE-1, false, true, vec_asteroids, s_r);
	}

	// create player
	player = std::make_unique<Player>(screen_center_x, screen_center_y, s_r);

	// Create main asteroids
	for (int i = 0; i < level[current_level]; i++)
	{
		x = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_WIDTH;;
		y = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_HEIGHT;
		CreateAsteroid(x, y, Asteroid::LARGE, true, false, vec_asteroids, s_r);
	}


	gScore = LoadRenderedText("SCORE: " + std::to_string(score), gtext_color, g_font, score_rect);
	score_rect.x = SCREEN_WIDTH/2 - score_rect.w / 2;
	score_rect.y += 10 * s_r;;
}

Game::~Game()
{
	exit_game();
}