#include "Game.h"
#include "Asteroid.h"
#include "Bullet.h"
#include <SDL_image.h>
#include <time.h>
#include "math.h"


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


void Game::splash_screen(const int time_to_display)
// Displays and delays the splash screen logo
{

	SDL_RenderCopy(gRenderer, game_images[eImages::SPLASH], NULL, NULL);

	//Update screen
	SDL_RenderPresent(gRenderer);
	SDL_Delay(time_to_display * 1000);
}


SDL_Texture* Game::LoadRenderedText(SDL_Texture* texture, std::string textureText, SDL_Color& textColor, TTF_Font* font, SDL_Rect& rect)
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
			
			rect = SDL_Rect{ rect.x, rect.y, textSurface->w, textSurface->h };

		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
	}
	return temp;
}


void Game::CreateAsteroid(double x, double y, float size, bool isCollidable, bool allowed_near_player, std::vector<std::unique_ptr<Entity>>& vector, float s_r)
{
	int angle;
	double vx;
	double vy;
	double rand_img;

	// randomise starting angle of image
	angle = ((double)rand() / (double)RAND_MAX) * 360;

	// randomise radian angle of velocity
	vx = ((double)rand() / (double)RAND_MAX) * 6.283185f;
	vy = ((double)rand() / (double)RAND_MAX) * 6.283185f;
	// calculate vector from this radian angle
	vx = sin(vx) * (((150.0*s_r) * ((double)rand() / (double)RAND_MAX)) + 60.0);
	vy = cos(vx) * (((150.0*s_r) * ((double)rand() / (double)RAND_MAX)) + 60.0);

	if (isCollidable)
	{	
		// random number to pic between 2 asteroid images
		rand_img = ((float)rand() / (float)RAND_MAX);
	}
	else
	{
		rand_img = NULL;
	}

	// If these asteroids cant spawn near the player (new wave etc)
	if (!allowed_near_player)
	{
		// create temp entity
		Asteroid temp(x, y, vx, vy, angle, size, rand_img, isCollidable, s_r);
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
	vector.push_back(std::make_unique<Asteroid>(x, y, vx, vy, angle, size, rand_img, isCollidable, s_r));
}


void Game::Create2SubAsteroids(const Asteroid* const ast, std::vector<std::unique_ptr<Entity>>& vector)
{
	// take radian angle of asteroid the 2 new ones will split from
	double radians = atan2(ast->vel_x, ast->vel_y);
	double radians2 = radians;

	// calculate an offset (max 1/4*PI (90 degrees)
	double offset = (PI / 4.0) * ((double)rand() / (double)RAND_MAX);
	// make sure they cant be too close to 0
	if (abs(offset) < .2) { offset = .2; }
	// set one to plus and other to minus of the offset (to imatate them splitting uniformly)
	radians += offset;
	radians2 -= offset;
	float distance = sqrtf(powf(ast->vel_x, 2) + powf(ast->vel_y, 2));
	// Calculate the new velocity vector from these angles
	double vx = sin(radians) * distance;
	double vy = cos(radians) * distance;
	double vx2 = sin(radians2) * distance;
	double vy2 = cos(radians2) * distance;

	// randomise the image thats used (out of 2 possible images)
	double rand_img = ((float)rand() / (float)RAND_MAX);

	// push the asteroids into the given vector
	vector.push_back(std::make_unique<Asteroid>(ast->pos_x, ast->pos_y, vx, vy, ast->angle, ast->size - 2.0, rand_img, ast->isCollidable, s_r));
	vector.push_back(std::make_unique<Asteroid>(ast->pos_x, ast->pos_y, vx2, vy2, ast->angle, ast->size - 2.0, rand_img, ast->isCollidable, s_r));
}

void Game::CreateBackgroundAsteroids(int amount)
{
	double x, y;
	for (int i = 0; i < amount/3; i++)
	{
		x = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_WIDTH;;
		y = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_HEIGHT;
		CreateAsteroid(x, y, Asteroid::SMALL - 1, false, true, vec_asteroids, s_r);

		x = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_WIDTH;;
		y = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_HEIGHT;
		CreateAsteroid(x, y, Asteroid::MEDIUM - 1, false, true, vec_asteroids, s_r);

		x = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_WIDTH;;
		y = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_HEIGHT;
		CreateAsteroid(x, y, Asteroid::LARGE - 1, false, true, vec_asteroids, s_r);
	}
}


void Game::handle_wave_completion()
{
	wave_complete = true;

	////if all levels complete
	//if (level.size() - 1 == current_level)
	//{
	//	// load game complete texture 
	//	if (lComplete == nullptr)
	//	{
	//		complete.x = SCREEN_WIDTH / 2 - complete.w / 2;
	//		complete.y = SCREEN_HEIGHT / 2 - complete.h / 2;
	//		//wave_delay.last_success = wave_delay.current_check = SDL_GetTicks();
	//		lComplete = LoadRenderedText("GAME COMPLETE - WIP", gtext_color, l_font, complete);
	//	}
	//	std::cout << "YOU WON" << std::endl;
	//	// Push scores is in top 3
	//	if (score > high_score.top3[2].second)
	//	{
	//		std::string name;
	//		std::cout << "\nYou scored in the top 3. Please type your name(no spaces): " << std::endl;
	//		SDL_RenderCopy(gRenderer, lComplete, NULL, &complete);
	//		SDL_RenderPresent(gRenderer);
	//		std::cin >> name;
	//		high_score.PushScore(name, score);
	//		high_score.Print();
	//		set_running(false);
	//	}
	//	else
	//	{
	//		std::cout << "\nYou didn't score in the top 3. Better luck next time\n" << std::endl;
	//		high_score.Print();
	//		SDL_RenderCopy(gRenderer, lComplete, NULL, &complete);
	//		SDL_RenderPresent(gRenderer);
	//		//SDL_Delay(3000.0);
	//		set_running(false);
	//	}

	//	SDL_DestroyTexture(lComplete);
	//	lComplete = nullptr;
	//	
	//}
	// //if more levels to go
	//else if (level.size() - 1 > current_level)

	// Simple endless mode
	wComplete = LoadRenderedText(wComplete, "WAVE " + std::to_string(current_level) + " COMPLETE", gtext_color, l_font, complete);
	complete.x = SCREEN_WIDTH / 2 - complete.w / 2;
	complete.y = SCREEN_HEIGHT / 2 - complete.h / 2;
	double x, y;
	current_level++;
	for (int i = 0; i < asteroid_amount + current_level; i++)
	{
		x = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_WIDTH;
		y = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_HEIGHT;
		CreateAsteroid(x, y, Asteroid::LARGE, true, false, vec_asteroids, s_r);
	}
	wave_delay.Reset();
	while (!wave_delay.DelayComplete(true))
	{
		SDL_RenderCopy(gRenderer, wComplete, NULL, &complete);
		SDL_RenderPresent(gRenderer);
	}

	SDL_DestroyTexture(wComplete);
	wComplete = nullptr;
}


void Game::handle_death()
{
	// render text textures and rects

	death1 = LoadRenderedText(death1, "YOU DIED", gtext_color, l_font, death1_rect);
	death2 = LoadRenderedText(death2, "Play again? (y/n)", gtext_color, g_font, death2_rect);
	death1_rect.x = SCREEN_WIDTH / 2 - death1_rect.w / 2;
	death1_rect.y = SCREEN_HEIGHT / 4 - death1_rect.h / 2;
	death2_rect.x = SCREEN_WIDTH / 2 - death2_rect.w / 2;
	death2_rect.y = SCREEN_HEIGHT - SCREEN_HEIGHT / 4 - death2_rect.h / 2;

	score_rect = death1_rect;

	score_rect.y = SCREEN_HEIGHT/2 - score_rect.h/2;
	float temp_dt = 1000/FPS;
	double zero_dt = 0.0;
	DelayTimer delta(temp_dt, false);
	// new game loop where everything updates and render at set FPS
	while (play_again == NULL)
	{
		if (delta.DelayComplete(true))
		{
			SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
			SDL_RenderClear(gRenderer);
			handle_input(zero_dt);
			for (auto& ast : vec_asteroids)
			{
				if (ast.get() != player->asteroid_death->get())
				{
					ast->Update(temp_dt / 1000);
					ast->WrapCoords(SCREEN_WIDTH, SCREEN_HEIGHT);
				}
				else
				{
					ast->Update(0.0);
				}
				ast->Draw();
			}
			player->Update(0.0);
			player->Draw();
			SDL_RenderCopy(gRenderer, gScore, NULL, &score_rect);
			SDL_RenderCopy(gRenderer, death1, NULL, &death1_rect);
			SDL_RenderCopy(gRenderer, death2, NULL, &death2_rect);
			SDL_RenderPresent(gRenderer);
		}
	}
	SDL_DestroyTexture(death1);
	SDL_DestroyTexture(death2);
}


void Game::handle_input(double& dt)
{
	//dt > 2 ? dt = 0 : dt = dt;
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
				break;
			case SDLK_w:
				player->img = Game::game_images[Game::eImages::SHIP_THRUST];
				break;
			case SDLK_i:
				player->debug = !(player->debug);
				for (auto& e : vec_asteroids)
				{
					e->debug = player->debug;
				}
				break;
			case SDLK_UP:
				player->img = Game::game_images[Game::eImages::SHIP_THRUST];
				break;
			case SDLK_y:
				if (player->is_dead)
				{
					// reset game
					play_again = 'y';
					set_running(false);
				}
				break;
			case SDLK_n:
				if (player->is_dead)
				{
					// end game
					play_again = 'n';
					set_running(false);
				}
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
			case SDLK_a:
				player->to_rotate = 0;
				break;
			case SDLK_LEFT:
				player->to_rotate = 0;
				break;
			case SDLK_d:
				player->to_rotate = 0;
				break;
			case SDLK_RIGHT:
				player->to_rotate = 0;
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
		player->to_rotate -= ANGLE_MODIFIER;
		//player->angle -= 
		//player->angle = player->angle % 360;
	}
	if (keystates[SDL_SCANCODE_D] || keystates[SDL_SCANCODE_RIGHT])
	{
		player->to_rotate += ANGLE_MODIFIER;

		//player->angle += ANGLE_MODIFIER * dt;
		//player->angle = player->angle % 360;
	}
	if (keystates[SDL_SCANCODE_SPACE] && (vec_asteroids.size() > 45))
	{
		if (shot_delay.DelayComplete(true))
		{
			vec_bullets.push_back(std::make_unique<Bullet>(player.get(), s_r));
		}
	}
}


void Game::Update(double& dt)
// Main object update loop for the game
{
	// check collisions between player and asteroids
	if (vec_asteroids.size() != 0)
	{
		for (auto& ast : vec_asteroids)
		{
			if (ast->isCollidable)
			{
				if ((player->Collision(*ast)) && (collision_delay.DelayComplete(true)))
				{
					score -= 500;
					if (score < 0) {score = 0; }
					gScore = LoadRenderedText(gScore, "SCORE: " + std::to_string(score), gtext_color, g_font, score_rect);

					if (current_sheild == 0)
					{
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
						player->is_dead = true;
						player->asteroid_death = &ast;
						handle_death();
					}
					else
					{
						// reduce sheild
						player->damaged = true;
						current_sheild -= .25;
						if (current_sheild < 0) { current_sheild = 0; }
						sheild_amount_rect->w = original_sheild_length * current_sheild;
					}
				}
			}

			// update and wrap coords on each asteroid
			ast->Update(dt);
			ast->WrapCoords(SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
	// Update player and Wrap coords
	player->Update(dt);
	player->WrapCoords(SCREEN_WIDTH, SCREEN_HEIGHT);
	if (player->damaged)
	{
		if (collision_delay.DelayComplete(false))
		{
			player->damaged = false;
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
							gScore = LoadRenderedText(gScore, "SCORE: " + std::to_string(score), gtext_color, g_font, score_rect);
							if (ast->size > Asteroid::SMALL)
							{
								Create2SubAsteroids((Asteroid*)(ast.get()), vec_tempAsteroids);
							}
							break;
						}
					}
				}
				// update bullet location
				bul->Update(dt);
				// check if bullet is outside of screen (set isDead to trye if it is outside screen)
				bul->WrapCoords(SCREEN_WIDTH, SCREEN_HEIGHT);
			}
		}

		// merge any new spawned asteroids into the main vector
		if (vec_tempAsteroids.size() != 0)
		{
			std::move(std::begin(vec_tempAsteroids), std::end(vec_tempAsteroids), std::back_inserter(vec_asteroids));
		}
	}

	// If bullet/asteroid is marked as dead, remove them
	vec_bullets.erase(std::remove_if(vec_bullets.begin(), vec_bullets.end(), [](std::unique_ptr<Entity>& e) { return (e->is_dead); }), vec_bullets.end());
	vec_asteroids.erase(std::remove_if(vec_asteroids.begin(), vec_asteroids.end(), [](std::unique_ptr<Entity>& e) { return (e->is_dead); }), vec_asteroids.end());


	// IF KILLED ALL ASTEROIDS
	// if only background asteroids remain and no bullets on screen
	if ((vec_asteroids.size() == 45) && (vec_bullets.empty()))
	{	
		handle_wave_completion();
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
	HudDraw();
}

void Game::HudDraw()
{
	SDL_RenderCopy(Game::gRenderer, sheild_bg_bar, NULL, sheild_bar_rect);
	SDL_SetRenderDrawColor(Game::gRenderer, 204, 255, 255, 255);
	SDL_RenderFillRect(Game::gRenderer, sheild_amount_rect);
}


Game::Game(const int FPS)
{
	this->FPS = FPS;

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

	// create background asteroids
	CreateBackgroundAsteroids(45);

	// create player
	player = std::make_unique<Player>(screen_center_x, screen_center_y, s_r);

	// Create main asteroids
	double x, y;
	for (int i = 0; i < asteroid_amount+ current_level; i++)
	{
		x = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_WIDTH;;
		y = (((double)rand() / (double)RAND_MAX)) * Game::SCREEN_HEIGHT;
		CreateAsteroid(x, y, Asteroid::LARGE, true, false, vec_asteroids, s_r);
	}

	//load text texture for score
	gScore = LoadRenderedText(gScore, "SCORE: " + std::to_string(score), gtext_color, g_font, score_rect);
	//scale rect
	score_rect.x = SCREEN_WIDTH/2 - score_rect.w / 2;
	score_rect.y += 10 * s_r;

	//set image for sheild HUD
	sheild_bg_bar = Game::game_images[Game::eImages::SHEILD_BG_BAR];
	// get the rects
	sheild_bar_rect = GetRect(sheild_amount, 18.0f * s_r, 18.0f * s_r);
	sheild_amount_rect = GetRect(sheild_amount, 28.0f * s_r, 28.0f * s_r);
	// scale using screen height and width
	sheild_bar_rect->h = SCREEN_HEIGHT / 25;
	sheild_bar_rect->w = SCREEN_WIDTH / 2.5;
	sheild_amount_rect->h = sheild_bar_rect->h - 20.0f * s_r;
	sheild_amount_rect->w = sheild_bar_rect->w - 20.0f * s_r;
	// save original size
	original_sheild_length = sheild_amount_rect->w;
	// scale the shown sheild amount by current sheild amount (1 = 100%, 0.5 = 50% etc)
	sheild_amount_rect->w = original_sheild_length * current_sheild;
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

	SDL_DestroyTexture(wComplete);
	wComplete = nullptr;
	
	SDL_DestroyTexture(death1);
	death1 = nullptr;
	SDL_DestroyTexture(death2);
	death2 = nullptr;

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
	SDL_Quit();
}


Game::~Game()
{
	exit_game();
}