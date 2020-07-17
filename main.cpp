#include "Game.h"

int main(int argc, char* argv[])
{
	char again;
	// Game 
	do
	{
		const int FPS = 60;
		double frame_delay = 1000.0 / FPS;

		Game game{FPS}; // initialize an object of the Game class

		Uint32 frame_start;
		Uint32 frame_time;
		double delta_time = 0.0;

		if (game.is_running())
		{
			game.splash_screen(2.5);
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
			else if (game.wave_complete) // If screen has been delayed and showing a message (reset delta so the new asteroids do not get updated by their vel* a huge delta time amount)
			{
				delta_time = 0.0;
				game.wave_complete = false;
			}
			else
			{
				delta_time = frame_time / 1000.0;
			}
		}

		again = game.play_again;

	} while (again == 'y');

	return 0;
}
