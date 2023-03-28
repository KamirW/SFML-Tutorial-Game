#include "main.h"
#include "Game.h"

int main() {
	
	std::cout << "Hello World! From SFML!\n" << std::endl;

	// Init random
	std::srand(static_cast<unsigned>(time(NULL))); 

	// Init Game Engine
	Game game;


	// Game Loop
	while (game.running() && !game.isGameEnded()) {

		// Update
		game.update();

		// Render
		game.render();

	}

	// End of application

	return 0;
}