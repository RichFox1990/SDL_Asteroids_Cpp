#include "Game.h"



using namespace std;

int main(int argc, char* argv[])
{
	// Game 
	Game game{}; // initialize an object of the Game class

	const int FPS = 60;
	double frame_delay = 1000.0 / FPS;

	Uint32 frame_start;
	Uint32 frame_time;
	double delta_time = 0.0;

	if (game.is_running())
	{
		game.splash_screen(1);
		cout << "Welcome";
	}

	while (game.is_running())
		{
			frame_start = SDL_GetTicks();

			game.handle_input(delta_time);
			game.Update(delta_time);
			game.render();

			frame_time = SDL_GetTicks() - frame_start;

			if (frame_time < frame_delay)
			{
				SDL_Delay(frame_delay - frame_time);
				delta_time = frame_delay/1000.0;
			}
			else
			{
				delta_time = frame_time / 1000.0;
			}

			//std::cout << "DT: " << delta_time << " || FT: " << frame_time/1000.0 << std::endl;
		}

	// memory leak test
	/*std::string text;
	while (std::cin >> text)
	{
		Game text{};
	}*/

	return 0;
}
