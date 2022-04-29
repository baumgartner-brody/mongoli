#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>

#include "Game.h"

#include <ctime>

int main(int argv, char** args) {

	Game *game = new Game();
	game->init("WINDOW NAME", 800, 600, false);

	while (game->running()) {
		game->handleEvents();
		game->update();
		game->draw();
	}

	game->clean();

	delete game;

	return 0;
}