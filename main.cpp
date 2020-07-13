#include "Game.h"

int main(int argc, char* argv[])
{
	char again;
	// Game 
	do
	{
		Game game{}; // initialize an object of the Game class

		const int FPS = 60;
		double frame_delay = 1000.0 / FPS;

		Uint32 frame_start;
		Uint32 frame_time;
		double delta_time = 0.0;

		if (game.is_running())
		{
			game.splash_screen(2.5);
			std::cout << "Welcome\n\n";
		}

		game.high_score.Print();

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
				delta_time = frame_delay / 1000.0;
			}
			else
			{
				delta_time = frame_time / 1000.0;
			}
		}

		std::cout << std::endl << "\nDo you want to play again? (y/n): ";
		std::cin >> again;

	} while (again == 'y');

	return 0;
}
