#include "Game.h"
#include "time.h"

int main(int argc, char* argv[])
{
	srand(time(NULL));
	
	const int FPS = 60;

	Game game{FPS}; // initialize an object of the Game class

	game.high_score.Print();
	
	while (game.is_running())
	{
		game.Run();
	}

	return 0;
}
